#include "include.hh"
#include "cam.hh"
#include "mat.hh"
#include "xray.hh"
#include "xhitobj.hh"
#include <iostream>
int main()
{
    Cam cc; //camera
    xHitObj_List world; // world objs
    shared_ptr<xMat> mground = make_shared<Lambertian>(XRGB{{0.75,0.75,0.0}});
    shared_ptr<xMat> ms1 = make_shared<Metal>(XRGB{{0.8,0.8,0.8}});
    shared_ptr<xMat> ms2 = make_shared<Lambertian>(XRGB{{0.7,0.3,0.3}});
    shared_ptr<xMat> ms3 = make_shared<Metal>(XRGB{{0.6,0.8,0.2}});
    world.add(make_shared<xSphere>(XV3{{0,-100.5,1}},100,mground));
    world.add(make_shared<xSphere>(XV3{{-1,0,-1}},0.5,ms1));
    world.add(make_shared<xSphere>(XV3{{0,0,-1}},0.5,ms2));
    world.add(make_shared<xSphere>(XV3{{1.0,0,-1}},0.5,ms3));
    cc.render(world); // render, output to img file
    return 0;
}
