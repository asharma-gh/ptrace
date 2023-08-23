#include "include.hh"
#include "cam.hh"
#include "xray.hh"
#include "xhitobj.hh"
#include <iostream>
int main()
{
    Cam cc; //camera
    xHitObj_List world; // world objs
    world.add(make_shared<xSphere>(XV3{{0,0,-1}},0.5));
    world.add(make_shared<xSphere>(XV3{{0,-100.5,1}},100));
    cc.render(world); // render, output to img file
    return 0;
}
