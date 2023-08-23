#pragma once
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>
#include "xtensor/xarray.hpp"
#include "xtensor/xmath.hpp"
#include "xtensor/xio.hpp"
#include "xtensor-blas/xlinalg.hpp"
///////////////////////////////////////
template<typename ...Args>
void 
LOG(Args && ...args)
{
    (std::clog<< ... <<args)<<std::endl<<std::flush;
}
///////////////////////////////////////
#define XV3 xt::xarray<double>
#define XV3_INIT(v) v = {{0,0,0}}
#define XRGB XV3
#define XRGB_INIT(v) XV3_INIT(v)
#define xR(v) (v[0])
#define xG(v) (v[1])
#define xB(v) (v[2])
namespace xVec3 {
    double norm_sq(const XV3& v) 
    {
        return xt::sum<double>(v*v)[0];   
    }
    double norm(const XV3& v)
    {
        return sqrt(norm_sq(v));
    }
    double dot(const XV3& v1, const XV3& v2)
    {
        return xt::sum<double>(v1*v2)[0];
    }
    XV3 cross(const XV3& v1, const XV3& v2)
    {
        return XV3{
            {
                (v1[1]*v2[2])-(v1[2]*v2[1]),
                (v1[0]*v2[2])-(v1[2]*v2[0]),
                (v1[0]*v2[1])-(v1[1]*v2[0])
            }
        };      
    }
}
namespace xRGB {
    using std::string;
    using std::stringstream;
    string to_string(const XRGB& v)
    {
        stringstream ss;
        ss<<static_cast<int>(xR(v)*255.99)<<' '
          <<static_cast<int>(xG(v)*255.99)<<' '
          <<static_cast<int>(xB(v)*255.99);
        return ss.str();
    }
}

///////////////////////////////////////
struct xRay {
    public:
        xRay(){}
        xRay(const XV3& pt, const XV3& dir)
            : _pt(pt),_dir(dir) {}
        XV3 pt()  const { return _pt;  }
        XV3 dir() const { return _dir; }
        XV3 at(double t) const { return _pt+(t*_dir); }
        XRGB color() const;
    private:
        XV3 _pt;
        XV3 _dir;
};
///////////////////////////////////////
using std::optional;
struct xHitRec {
    XV3 pt;
    XV3 snorm;
    double t;
    bool is_front;
    // update so norm always point outward, back to cam
    void update_front_norm(const xRay& r, const XV3& out_norm)
    {
        is_front = dot(r.dir(), out_norm) < 0;
        snorm=is_front ? out_norm : -out_norm;
    }
};
struct xHitObj {
// objects which can be hit by rays
public:
    virtual ~xHitObj()=default;
    // r: ray, tmin: low t interval, tmax: high t interval ((tmin,tmax))
    // rec: hit record
    virtual bool hit(const xRay& r, double tmin, double tmax, xHitRec& rec) = 0;
};
struct xHitObj_List : xHitObj {
// collection of multiple hittable objects
using std::make_shared;
using std::shared_ptr;
using std::vector;
public:
    xHitObj_List(){}
    xHitObj_List(shared_ptr<xHitObj> obj) { add(obj); }
    void clear() { _objs.clear(); }
    void add(shared_ptr<xHitObj> obj)
    {
        _objs.push_back(obj);
    }
    bool hit(const xRay& r, double tmin, double tmax, xHitRec& rec) override 
    {
        double cur_dist=tmax;
        bool did_hit=0;
        xHitRec temp;
        // each iteration, update tmax so rec stores the nearest object hit
        // Temp code
        for (const auto& obj: _obs)
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
struct xSphere : xHitObj {
    XV3 center;
    double rad;
    xSphere(){}
    xSphere(XV3 c,double r)
        : center(c),rad(r){}
    bool hit(const xRay& r, double tmin, double tmax, xHitRec& rec) override
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
XRGB xRay::color() const 
{
    xSphere s(XV3{{0,0,-1}},0.5);
    xHitRec rec;
    if (s.hit(*this, 0, 100, rec))
    {
        // convert domain of [-1,1] to [0,1]
        return 0.5*XV3{{rec.snorm[0]+1,rec.snorm[1]+1,rec.snorm[2]+1}};
    }
    XV3 unitd=this->_dir/xVec3::norm(this->_dir);
    double yh=0.5*(unitd[1] + 1);
    // linear interp 1->full blue, 0->white
    return (1-yh)*XRGB{{1,1,1}} + yh*XRGB{{0.5,0.6,1.0}};
}
