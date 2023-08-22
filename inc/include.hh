#pragma once
#include "xtensor/xarray.hpp"
#include "xtensor/xmath.hpp"
#include "xtensor/xio.hpp"
#include "xtensor-blas/xlinalg.hpp"
///////////////////////////////////////
template<typename ...Args>
void 
LOG(Args && ...args)
{
    (std::clog<< ... <<args)<<std::flush;
}
///////////////////////////////////////
#define XV3 xt::xarray<double>
#define XRGB XV3
#define XRGB_INIT(v) v = {{0,0,0}}
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
        return (v1[0]*v2[0])+(v1[1]*v2[1])+(v1[2]*v2[2])
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
            : pt(pt),dir(dir) {}
        XV3 get_pt()  const { return pt;  }
        XV3 get_dir() const { return dir; }
        XV3 at(double t){ return pt+(t*dir); }
    private:
        XV3 pt;
        XV3 dir;
};
///////////////////////////////////////

