#include "myolinux/myoclient.h"
#include "myolinux/serial.h"
#include <unistd.h>
#include <cinttypes>
#include <sys/time.h>
#include "libowi.hpp"

using namespace myolinux;

long long lastUpdate = 0;
bool shouldVibrate = false;

long long getCurrentTimestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

int main()
{
    myo::Client client(Serial{"/dev/ttyACM0", 115200});
    owi_init();
    // Autoconnect to the first Myo device
    client.connect();
    if (!client.connected()) {
        return 1;
    }

    // Print device address
    print_address(client.address());

    // Read firmware version
    auto version = client.firmwareVersion();
    std::cout << version.major << "."
        << version.minor << "."
        << version.patch << "."
        << version.hardware_rev << std::endl;

    // Vibrate
    client.vibrate(myo::Vibration::Medium);
    // Read name
    auto name = client.deviceName();
    std::cout << name << std::endl;

    // Set sleep mode (otherwise the device auto disconnects after a while)
    client.setSleepMode(myo::SleepMode::NeverSleep);

    // Read EMG and IMU
    client.setMode(myo::EmgMode::SendEmg, myo::ImuMode::SendData, myo::ClassifierMode::Enabled);

    client.onEmg([](myo::EmgSample sample)
    {
      if(getCurrentTimestamp() - lastUpdate > 50) {
         lastUpdate = getCurrentTimestamp();
        for (std::size_t i = 0; i < 8; i++) {
            std::cout << static_cast<int>(sample[i]);
            if (i != 7) {
                std::cout << "\t";
            }
            if( static_cast<int>(sample[i]) > 80) {
               for(unsigned int i = 0; i < 10 ; i++)
                  std::cout << std::endl;
               std::cout << "PRESS" << std::endl;
               shouldVibrate = true;

            }
        }
        std::cout << std::endl;
     }
  });

   /*client.onImu([](myo::OrientationSample ori, myo::AccelerometerSample acc, myo::GyroscopeSample gyr)
    {
        std::cout << "Ori : ";
        std::cout << ori[0] << "\t" << ori[1] << "\t" << ori[2] << "\t" <<  ori[3] << "\tAcc : ";
        std::cout << acc[0] << "\t" << acc[1] << "\t" << acc[2]  << "\tGyro : ";
        std::cout << gyr[0] << "\t" << gyr[1] << "\t" << gyr[2] << std::endl;
    });*/
   long long lastVibrate = getCurrentTimestamp();
   bool lastWasOpen = false;
    while(true) {
        client.listen();
        if(shouldVibrate && getCurrentTimestamp() - lastVibrate > 500) {
           client.vibrate(myo::Vibration::Short);
           shouldVibrate = false;
           lastVibrate = getCurrentTimestamp();
           lastWasOpen = !lastWasOpen;
           if(lastWasOpen)
               owi_m1_close();
            else
               owi_m1_open();
            sleep(1);
            owi_m1_off();
        }
    }

    client.disconnect();
}
