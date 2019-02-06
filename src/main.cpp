#include <myolinux/myoclient.h>
#include <myolinux/serial.h>
#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "utils.hpp"
#include "myoband.hpp"
#include "owiarm.hpp"
#include "ecginterpreter.hpp"

using namespace std;
using namespace myolinux;
using namespace sf;


int main() {
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "yArm");
    // MyoBand band;
    OwiArm arm;
    EcgInterpreter interpreter;
    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.draw(interpreter);
        window.display();
    }
/*
    cout << arm << endl;
    for(unsigned int i = 0 ; i < 10e9 ; i++) {
        // band.update();
        // Vec4f orientation = band.getOrientation();
        // arm.setTargetOrientation(toEulerAngles(orientation));

        arm.setTargetOrientation({-3.14/2,-3.14/2,0});
        arm.update();
        if(i % 100000000) cout << arm << endl;
        //sleepMS(1000);
    }
    */
}
