#pragma once
#include <optional>
#include <iostream>
#include <sstream>
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
struct xSphere {
    XV3 center;
    double rad;
    xSphere(){}
    xSphere(XV3 c,double r)
        : center(c),rad(r){}
    optional<XV3> hit(const xRay& r)
    {
        XV3 to=r.pt() - center;
        //quadratic form
        double a=xVec3::dot(r.dir(),r.dir());
        double b_half=xVec3::dot(to,r.dir());
        double c=xVec3::dot(to,to)-(rad*rad);
        double disc=(b_half*b_half)-(a*c);
        if (disc < 0)
            return std::nullopt;
        // pick low term
        double xint= (-b_half - sqrt(disc)) / a;
        // xint contains the value where r intersects s,
        // minus sphere center to get surface norm
        XV3 snorm=r.at(xint)-this->center;
        // return unit norm
        return optional<XV3>{snorm / xVec3::norm(snorm)};
    }
};
XRGB xRay::color() const 
{
    xSphere s(XV3{{0,0,-1}},0.5);
    optional<XV3> osnorm = s.hit(*this);
    if (osnorm.has_value())
    {
        XV3 snorm=osnorm.value();
        // convert domain of [-1,1] to [0,1]
        return 0.5*XV3{{snorm[0]+1,snorm[1]+1,snorm[2]+1}};
    }
    XV3 unitd=this->_dir/xVec3::norm(this->_dir);
    double yh=0.5*(unitd[1] + 1);
    // linear interp 1->full blue, 0->white
    return (1-yh)*XRGB{{1,1,1}} + yh*XRGB{{0.5,0.6,1.0}};
}
