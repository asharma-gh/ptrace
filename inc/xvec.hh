#pragma once
#include "include.hh"
///////////////////////////////////////
//xt macros
#define XV3 xt::xarray<double>
#define XV3_INIT(v) v = {{0,0,0}}
#define XRGB XV3
#define XRGB_INIT(v) XV3_INIT(v)
#define xR(v) (v[0])
#define xG(v) (v[1])
#define xB(v) (v[2])
///////////////////////////////////////
namespace xVec3 {
    inline double norm_sq(const XV3& v) { return xt::sum<double>(v*v)[0]; }
    inline double norm(const XV3& v) { return sqrt(norm_sq(v)); }
    inline double dot(const XV3& v1, const XV3& v2) { return xt::sum<double>(v1*v2)[0]; }
    inline XV3 cross(const XV3& v1, const XV3& v2)
    {
        return XV3{{
                (v1[1]*v2[2])-(v1[2]*v2[1]),
                (v1[0]*v2[2])-(v1[2]*v2[0]),
                (v1[0]*v2[1])-(v1[1]*v2[0])
            }};  
    }
    inline void zero(XV3& v) { v[0]=0; v[1]=0; v[2]=0; }
    inline bool near_zero(const XV3& v) 
    {
        double er=1e-8;
        return (fabs(v[0]) < er) && (fabs(v[1]) < er) && (fabs(v[2]) < er);
    }
    inline XV3 reflect(const XV3& v, const XV3& n)
    {
        // v + -1*2*[proj. v->n]. note: v pts in opposite dir to n
        return v-(2*xVec3::dot(v,n)*n);
    }
    inline XV3 refract(const XV3& udir, const XV3& snorm, double refr)
    {
        double costh = fmin(dot(-udir,snorm),1.0); // udir,snorm are unit vec.
        XV3 r_perp = refr*(udir+(costh*snorm));
        XV3 r_pl   = -1*sqrt(fabs(1.0 - xVec3::norm_sq(r_perp)))*snorm;
        return r_perp+r_pl;
    }
    inline XV3 unit_vec(const XV3& v) { return v / xVec3::norm(v); }
    inline XV3 random() { return XV3{{ran_d(),ran_d(),ran_d()}}; };
    inline XV3 random(double d1, double d2) 
    { 
        return XV3{{ran_d(d1,d2),ran_d(d1,d2),ran_d(d1,d2)}}; 
    }
    inline XV3 random_in_unit_sphere()
    {
        while(1)
        {
            XV3 v=xVec3::random(-1,1);
            if (xVec3::norm_sq(v) < 1)
                return v;
        }
    }
    inline XV3 random_unit_vec()
    {
        XV3 v=xVec3::random_in_unit_sphere();
        return xVec3::unit_vec(v);
    }
    inline XV3 random_on_hemisphere(const XV3& vnorm)
    {
        XV3 vs = random_in_unit_sphere();
        if (dot(vs,vnorm) > 0.0) // in same hs as vnorm
            return vs;
        else
            return -1*vs;
    }
};
///////////////////////////////////////
namespace xRGB {
    using std::clamp;
    using std::string;
    using std::stringstream;
    inline double lin_to_gam(double lin) { return sqrt(lin); };
    inline std::string to_string(const XRGB& v)
    {
        stringstream ss;
        ss<<static_cast<int>(clamp(xRGB::lin_to_gam(xR(v)),0.0,0.999)*256)<<' '
          <<static_cast<int>(clamp(xRGB::lin_to_gam(xG(v)),0.0,0.999)*256)<<' '
          <<static_cast<int>(clamp(xRGB::lin_to_gam(xB(v)),0.0,0.999)*256);
        return ss.str();
    }
};
///////////////////////////////////////

