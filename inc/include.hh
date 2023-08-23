#pragma once
#include <algorithm>
#include <cstdlib>
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
// math
const double infinity=std::numeric_limits<double>::infinity();
const double pi=3.1415926535897932385;

inline double ran_d()
{
    //[0,1)
    return (double)rand()/(RAND_MAX);
}
inline double ran_d(double d1, double d2)
{
    //[d1,d2)
    return d1+(d2-d1)*ran_d();
}
///////////////////////////////////////
