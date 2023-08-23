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
// math
const double infinity=std::numeric_limits<double>::infinity();
const double pi=3.1415926535897932385;

