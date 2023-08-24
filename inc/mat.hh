#pragma once
#include "include.hh"
#include "xvec.hh"
#include "xray.hh"
struct xHitRec;
///////////////////////////////////////
struct xMat {
    virtual ~xMat()=default;
    virtual bool scatter(
            const xRay& r_in, 
            const xHitRec& rec, 
            XRGB& atten, 
            xRay& scattered) const = 0;
};
///////////////////////////////////////
struct Lambertian : xMat {
public:
    Lambertian(const XRGB& al) 
        : _albedo(al) {} 
    bool scatter(
            const xRay& r_in, 
            const xHitRec& rec, 
            XRGB& atten, 
            xRay& scattered) const override
    {
        XV3 sdir = rec.snorm + xVec3::random_unit_vec();
        if (xVec3::near_zero(sdir)) // degen. case
            sdir = rec.snorm;
        scattered=xRay(rec.pt, sdir);
        atten=_albedo;
        return true;
    }
private:
    XRGB _albedo;
};
///////////////////////////////////////
struct Metal : xMat {
public:
    Metal(const XRGB& al, double df) 
        : _albedo(al), _diffuse(df < 1 ? df : 1) {} 
    bool scatter(
            const xRay& r_in, 
            const xHitRec& rec, 
            XRGB& atten, 
            xRay& scattered) const override
    {
        XV3 refl = xVec3::reflect(xVec3::unit_vec(r_in.dir()), rec.snorm);
        scattered=xRay(rec.pt, refl + _diffuse*xVec3::random_unit_vec());
        atten=_albedo;
        return (xVec3::dot(scattered.dir(), rec.snorm) > 0);
    }
private:
    XRGB   _albedo;
    double _diffuse;
};
///////////////////////////////////////
struct Dielectric : xMat {
public:
    Dielectric(double refr_index) 
        : _refr_index(refr_index) {} 
    bool scatter(
            const xRay& r_in, 
            const xHitRec& rec, 
            XRGB& atten, 
            xRay& scattered) const override
    {
        atten={{1.0,1.0,1.0}};
        double refr_ratio = rec.is_front ? (1.0/_refr_index) : _refr_index;
        XV3 unit_dir=xVec3::unit_vec(r_in.dir());
        //determine if can refract.
        double costh = fmin(xVec3::dot(-1*unit_dir,rec.snorm),1.0);
        double sinth = sqrt(1.0-(costh*costh)); //trig identity
        bool no_refr = refr_ratio*sinth > 1.0;
        XV3 dir;
        if (no_refr || reflectance(costh,refr_ratio) > ran_d())
            dir=xVec3::reflect(unit_dir, rec.snorm);
        else
            dir=xVec3::refract(unit_dir, rec.snorm, refr_ratio);
        scattered=xRay(rec.pt,dir);
        return true;
    }
private:
    double _refr_index;
    static inline double reflectance(double costh, double rf_id)
    {
        // Schlick approximation.
        double r0=(1-rf_id) / (1+rf_id);
        r0*=r0;
        return r0 + (1-r0)*(pow((1-costh),5));
    }
};
