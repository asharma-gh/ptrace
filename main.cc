#include "include.hh"
#include <iostream>
int main()
{
    using std::endl;
    using std::cout;
    //
    double aspect_ratio=16/9;
    int w=700;
    int h=static_cast<int>(w/aspect_ratio);
    h=(h<1)?1:h; //ensure h clamp to 1 if w<a_ratio
    double viewport_h=2;
    double viewport_w=viewport_h*(static_cast<double>(w)/h);
    // output to ppm
    cout<<"P3"<<endl;
    cout<<w<<' '<<h<<endl;
    cout<<"255"<<endl;
    XRGB v;
    XRGB_INIT(v);
    for(int ii=0;ii<h;ii++)
    {
        LOG("Rows remaining ... ",h-ii);
        for(int jj=0;jj<w;jj++)
        {
            xR(v) = double(jj)/(w-1); // max red=right
            xG(v) = double(ii)/(h-1); // max green=bottom
            xB(v) = 0;
            cout<<xRGB::to_string(v)<<endl;
        }
    }
    return 0;
}
