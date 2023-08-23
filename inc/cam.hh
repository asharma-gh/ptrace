#pragma once
#include "include.hh"
#include "xray.hh"
#include "xvec.hh"
#include "xhitobj.hh"
struct Cam {
public:
    Cam();
    void render(const xHitObj_List& world);
private:
    //output img
    double _a_ratio;
    int _img_w;
    int _img_h;
    //viewport dims
    double _vp_h;
    double _vp_w;
    //cam properties
    double _focal_l;
    XV3 _cam_center;
    XV3 _vp_u; //vp vec
    XV3 _vp_v;
    XV3 _p_delta_u; //pixel->vp delta
    XV3 _p_delta_v;
    XV3 _vp_orig; // viewport origin
    XV3 _pix00_pos;

    void _init();
    XRGB _r_color(const xRay& r, const xHitObj_List& world) const;
};
