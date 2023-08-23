#pragma once
#include "xvec.hh"
#include "include.hh"
///////////////////////////////////////
struct xHitObj_List; // fwd decl.
struct xRay {
    public:
        xRay(){}
        xRay(const XV3& pt, const XV3& dir)
            : _pt(pt),_dir(dir) {}
        XV3 pt()  const { return _pt;  }
        XV3 dir() const { return _dir; }
        XV3 at(double t) const { return _pt+(t*_dir); }
        XRGB color(const xHitObj_List& world) const;
    private:
        XV3 _pt;
        XV3 _dir;
};
