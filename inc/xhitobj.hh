#pragma once
#include "xvec.hh"
#include "xray.hh"
///////////////////////////////////////
struct xHitRec {
    XV3 pt;
    XV3 snorm;
    double t;
    bool is_front;
    // update so norm always point outward, back to cam
    void update_front_norm(const xRay& r, const XV3& out_norm)
    {
        is_front = xVec3::dot(r.dir(), out_norm) < 0;
        snorm=out_norm;
        if (not is_front)
            snorm *= -1;
    }
};
///////////////////////////////////////
struct xHitObj {
// objects which can be hit by rays
public:
    virtual ~xHitObj()=default;
    // r: ray, tmin: low t interval, tmax: high t interval ((tmin,tmax))
    // rec: hit record
    virtual bool hit(const xRay& r, double tmin, double tmax, xHitRec& rec) const = 0;
};

///////////////////////////////////////
using std::make_shared;
using std::shared_ptr;
using std::vector;
//
struct xHitObj_List : xHitObj {
// collection of multiple hittable objects
public:
    xHitObj_List(){}
    xHitObj_List(shared_ptr<xHitObj> obj) { add(obj); }
    void clear() { this->_objs.clear(); }
    void add(shared_ptr<xHitObj> obj)
    {
        this->_objs.push_back(obj);
    }
    bool hit(const xRay& r, double tmin, double tmax, xHitRec& rec) const override 
    {
        double cur_dist=tmax;
        bool did_hit=0;
        xHitRec temp;
        // each iteration, update tmax so rec stores the nearest object hit
        // Temp code
        for (const auto& obj: this->_objs)
        {
            if (obj->hit(r,tmin,cur_dist,temp))
            {
                did_hit = true;
                cur_dist=temp.t;
                rec=temp;
            }
        }
        return did_hit;
    }
private:
    vector<shared_ptr<xHitObj>> _objs;
};
///////////////////////////////////////
struct xSphere : xHitObj {
    XV3 center;
    double rad;
    xSphere(){}
    xSphere(XV3 c,double r)
        : center(c),rad(r){}
    bool hit(const xRay& r, double tmin, double tmax, xHitRec& rec) const override
    {
        XV3 to=r.pt() - center;
        //quadratic form
        double a=xVec3::dot(r.dir(),r.dir());
        double b_half=xVec3::dot(to,r.dir());
        double c=xVec3::dot(to,to)-(rad*rad);
        double disc=(b_half*b_half)-(a*c);
        if (disc < 0)
            return false;
        double sqdisc=sqrt(disc);
        double xint =(-b_half - sqdisc) / a;
        // return the minimum intercept within tmin,tmax
        if (xint >= tmax)
            return false;
        if (xint <= tmin)
        {
            // test high intercept
            xint=(-b_half + sqdisc) / a;
            if (xint <= tmin || xint >= tmax)
                return false;
        }
        // xint contains the value where r intersects s,
        // minus sphere center to get surface norm
        XV3 snorm=r.at(xint)-this->center;
        // update hitrec
        rec.t=xint;
        rec.pt=r.at(xint);
        rec.update_front_norm(r,snorm/rad);
        return true;
    }
};

