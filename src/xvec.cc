#include "xvec.hh"
double xVec3::norm_sq(const XV3& v) 
{
    return xt::sum<double>(v*v)[0];   
}
double xVec3::norm(const XV3& v)
{
    return sqrt(norm_sq(v));
}
double xVec3::dot(const XV3& v1, const XV3& v2)
{
    return xt::sum<double>(v1*v2)[0];
}
XV3 xVec3::cross(const XV3& v1, const XV3& v2)
{
    return XV3{
        {
            (v1[1]*v2[2])-(v1[2]*v2[1]),
            (v1[0]*v2[2])-(v1[2]*v2[0]),
            (v1[0]*v2[1])-(v1[1]*v2[0])
        }
    };      
}

using std::string;
using std::stringstream;
string xRGB::to_string(const XRGB& v)
{
    stringstream ss;
    ss<<static_cast<int>(xR(v)*255.99)<<' '
      <<static_cast<int>(xG(v)*255.99)<<' '
      <<static_cast<int>(xB(v)*255.99);
    return ss.str();
}

