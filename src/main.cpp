#include <myolinux/myoclient.h>
#include <myolinux/serial.h>
#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <math.h>

#include "utils.hpp"
#include "myoband.hpp"
#include "owiarm.hpp"
#include "ecginterpreter.hpp"

using namespace std;
using namespace myolinux;
using namespace sf;


int main() {

    MyoBand band;
    //RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "yArm");
   OwiArm arm;
   // band.vibrate(myolinux::myo::Vibration::);
    /*EcgInterpreter interpreter;
    unsigned int j = 0;
    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        double data[8];
        band.update();
        band.getEMGArray(data);
       // cout << "emgData 0" << data[0] << endl;
        interpreter.feedData(data);
        window.draw(interpreter);
        window.display();
    }*/


   for(unsigned int i = 0 ; i < 10e9 ; i++) {


      band.update();
      Vec4f orientation = band.getOrientation();
      Vec3f target = toEulerAngle(orientation);
      arm.setTargetOrientation(target);
      //arm.setTargetOrientation({-3.14/2,-3.14/2,0});
      arm.update();
      if(i % 100000000) {
         cout << "arm:\t"<< arm << endl;
         cout << "tar:\t" << target.x << "\t" << target.y << "\t" << target.z << endl;
        //sleepMS(1000);
      }

   
   }
   owi_shutdown();
}
