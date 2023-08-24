#include "cam.hh"
using std::cout;
using std::endl;
Cam::Cam()
{
    _init();
}
void Cam::_init()
{
    _a_ratio=16.0/9.0;
    _img_w=700;
    _img_h=static_cast<int>(static_cast<double>(_img_w)/_a_ratio);
    _img_h=(_img_h<1)?1:_img_h; //ensure h clamp to 1 if w<a_ratio
    _pix_samples=10;
    _r_max_d=10;
    _vp_h=2.0;
    _vp_w=_vp_h*(static_cast<double>(_img_w)/_img_h);
    _focal_l=1.0;
    XV3_INIT(_cam_center);
    _vp_u={{_vp_w,0,0}};
    _vp_v={{0,-1*_vp_h,0}};
    //delta to next pixel in viewport
    _p_delta_u=_vp_u/_img_w;
    _p_delta_v=_vp_v/_img_h;
    //origin of viewport in world coords
    _vp_orig=_cam_center - XV3{{0,0,_focal_l}} - _vp_u/2 - _vp_v/2;
    //add border between vp orig and top left pixel
    _pix00_pos=_vp_orig+(0.5*(_p_delta_u+_p_delta_v));
}
void Cam::render(const xHitObj_List& world)
{

    //output to ppm
    cout<<"P3"<<endl;
    cout<<_img_w<<' '<<_img_h<<endl;
    cout<<"255"<<endl;
    // sampled vars
    XRGB pcolor;
    XRGB_INIT(pcolor);
    xRay sample_r;
    sample_r.set_pt(_cam_center);
    XV3 pt_sample;
    for(int ii=0;ii<_img_h;ii++)
    {
        LOG("Rows remaining ... ",_img_h-ii);
        for(int jj=0;jj<_img_w;jj++)
        {
            xVec3::zero(pcolor);
            for(int sample=0;sample<_pix_samples;sample++)
            {
                _sample_ray_pt(sample_r,ii,jj,pt_sample);
                pcolor += _r_color(sample_r, _r_max_d, world);
            }
            // avg pcolor
            pcolor /= _pix_samples;
            cout<<xRGB::to_string(pcolor)<<endl;
        }
    }
}
void Cam::_sample_ray_pt(xRay& r, int ii, int jj, XV3& tmp)
{
    XV3 pixel_center=_pix00_pos+(jj*_p_delta_u)+(ii*_p_delta_v);
    _sample_pix_sq(tmp);
    tmp=(pixel_center+tmp) - _cam_center;
    r.set_dir(tmp);
}
void Cam::_sample_pix_sq(XV3& tmp)
{
    // [-0.5,0.5)
    double px=-0.5 + ran_d();
    double py=-0.5 + ran_d();
    tmp=((px*_p_delta_u) + (py*_p_delta_v));
}
XRGB Cam::_r_color(const xRay& r, int cur_depth, const xHitObj_List& world) const 
{
    if (cur_depth == 0)
        return XRGB{{0,0,0}}; // no light is found if no bounce occured 
                              // or in other words, if we bounced up to max_d, we have an occlusion shadow
    xHitRec rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        // convert domain of [-1,1] to [0,1]
        //return 0.5*XV3{{rec.snorm[0]+1,rec.snorm[1]+1,rec.snorm[2]+1}};
        // matte
        XV3 dir=rec.snorm + xVec3::random_unit_vec();
        // bounce ray 
        return 0.5*_r_color(xRay(rec.pt,dir), cur_depth-1, world);
    }
    // background
    XV3 unitd=r.dir()/xVec3::norm(r.dir());
    double yh=0.5*(unitd[1] + 1);
    // linear interp 1->full blue, 0->white
    return (1-yh)*XRGB{{1,1,1}} + yh*XRGB{{0.5,0.6,1.0}};
}
