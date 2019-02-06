#include "myoband.hpp"

using namespace myolinux;

MyoBand::MyoBand() : mClient(Serial{"/dev/ttyACM0", 115200})  {
    mOrientation = {0,0,0,0};
    // Autoconnect to the first Myo device
    mClient.connect();
    if (!mClient.connected()) {
        return;
    }

    // Print device address
    print_address(mClient.address());

    // Vibrate
    mClient.vibrate(myo::Vibration::Medium);

    // Set sleep mode (otherwise the device auto disconnects after a while)
    mClient.setSleepMode(myo::SleepMode::NeverSleep);

    // Read EMG and IMU
    mClient.setMode(myo::EmgMode::SendEmg, myo::ImuMode::SendData, myo::ClassifierMode::Enabled);

    mClient.onEmg([this](myo::EmgSample sample) {
        for(unsigned int i = 0 ; i < 7 ; i++) {
           mEmgData[i] = sample[i];
        }
    });

   mClient.onImu([this](myo::OrientationSample ori, myo::AccelerometerSample acc, myo::GyroscopeSample gyr) {
        mOrientation  = {
            static_cast<double>(ori[0]) / myo::OrientationScale, 
            static_cast<double>(ori[1]) / myo::OrientationScale, 
            static_cast<double>(ori[2]) / myo::OrientationScale, 
            static_cast<double>(ori[3]) / myo::OrientationScale
        };
        mAcceleration = {
            static_cast<double>(acc[0]) / myo::AccelerometerScale,
            static_cast<double>(acc[1]) / myo::AccelerometerScale, 
            static_cast<double>(acc[2]) / myo::AccelerometerScale
        };
        mAngularSpeed = {
            static_cast<double>(gyr[0]) / myo::GyroscopeScale, 
            static_cast<double>(gyr[1]) / myo::GyroscopeScale, 
            static_cast<double>(gyr[2]) / myo::GyroscopeScale
        };
    });
} 

void MyoBand::update() {
    mClient.listen();
}