#ifndef MYO_WRAPPER
#define MYO_WRAPPER

#include "utils.hpp"
#include <myolinux/myoclient.h>
#include <myolinux/serial.h>


class MyoBand {
    public:
    MyoBand();
    void update();
    void vibrate(myolinux::myo::Vibration vib) { 
        mClient.vibrate(vib);
    }
    // Accessors
    Vec4f getOrientation()  { return mOrientation; }
    Vec3f getAcceleration() { return mAcceleration; }
    Vec3f getAngularSpeed() { return mAngularSpeed; }
    void getEMGArray(double emgData[]) { emgData = mEmgData; }
    private:
    Vec4f       mOrientation;
    Vec3f       mAcceleration;
    Vec3f       mAngularSpeed;
    double      mEmgData[8];
    myolinux::myo::Client mClient;
};


#endif