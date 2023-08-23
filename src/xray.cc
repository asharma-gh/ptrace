#include "xray.hh"
#include "xhitobj.hh"
XRGB xRay::color(const xHitObj_List& world) const 
{
    xHitRec rec;
    if (world.hit(*this, 0, infinity, rec))
    {
        // convert domain of [-1,1] to [0,1]
        return 0.5*XV3{{rec.snorm[0]+1,rec.snorm[1]+1,rec.snorm[2]+1}};
    }
    // background
    XV3 unitd=this->_dir/xVec3::norm(this->_dir);
    double yh=0.5*(unitd[1] + 1);
    // linear interp 1->full blue, 0->white
    return (1-yh)*XRGB{{1,1,1}} + yh*XRGB{{0.5,0.6,1.0}};
}
