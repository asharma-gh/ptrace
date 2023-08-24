#pragma once
#include "include.hh"
#include "xvec.hh"
#include "xray.hh"
struct xHitRec;
///////////////////////////////////////
struct xMat {
    virtual ~xMat()=default;
    virtual bool scatter(
            const xRay& r_in, const xHitRec& rec, XRGB& atten, xRay& scattered) const = 0;
};
///////////////////////////////////////
struct Lambertian : xMat {
public:
        Lambertian(const XRGB& al) 
            : albedo(al) {} 
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
            atten=albedo;
            return true;
        }
private:
        XRGB albedo;
};
///////////////////////////////////////
struct Metal : xMat {
public:
        Metal(const XRGB& al) 
            : albedo(al) {} 
        bool scatter(
                const xRay& r_in, 
                const xHitRec& rec, 
                XRGB& atten, 
                xRay& scattered) const override
        {
            XV3 refl = xVec3::reflect(xVec3::unit_vec(r_in.dir()), rec.snorm);
            scattered=xRay(rec.pt, refl);
            atten=albedo;
            return true;
        }
private:
        XRGB albedo;
};
