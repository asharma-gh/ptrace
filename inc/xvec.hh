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
    double norm_sq(const XV3& v);
    double norm(const XV3& v);
    double dot(const XV3& v1, const XV3& v2);
    XV3    cross(const XV3& v1, const XV3& v2);
};
namespace xRGB {
    std::string to_string(const XRGB& v);
};

