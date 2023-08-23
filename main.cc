#include "include.hh"
#include <iostream>
int main()
{
    using std::endl;
    using std::cout;
    // output image
    //
    double aspect_ratio=16.0/9.0;
    int img_w=700;
    int img_h=static_cast<int>(static_cast<double>(img_w)/aspect_ratio);
    img_h=(img_h<1)?1:img_h; //ensure h clamp to 1 if w<a_ratio
    // camera
    //
    double viewport_h=2.0;
    double viewport_w=viewport_h*(static_cast<double>(img_w)/img_h);
    double focal_l=1.0;
    XV3 camera_center;
    XV3_INIT(camera_center);
    XV3 viewport_u={{viewport_w,0,0}};
    XV3 viewport_v={{0,-1*viewport_h,0}};
    //delta to next pixel in viewport
    XV3 p_delta_u=viewport_u/img_w;
    XV3 p_delta_v=viewport_v/img_h;
    //origin of viewport in world coords
    XV3 vp_orig=camera_center - XV3{{0,0,focal_l}} - viewport_u/2 - viewport_v/2;
    //add border between vp orig and top left pixel
    XV3 pixel00_pos=vp_orig+(0.5*(p_delta_u+p_delta_v));
    //output to ppm
    cout<<"P3"<<endl;
    cout<<img_w<<' '<<img_h<<endl;
    cout<<"255"<<endl;
    for(int ii=0;ii<img_h;ii++)
    {
        LOG("Rows remaining ... ",img_h-ii);
        for(int jj=0;jj<img_w;jj++)
        {
            XV3 p_center=pixel00_pos+(jj*p_delta_u)+(ii*p_delta_v);
            XV3 ray_dir=p_center-camera_center;
            xRay r(camera_center,ray_dir);
            cout<<xRGB::to_string(r.color())<<endl;
        }
    }
    return 0;
}
