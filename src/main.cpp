#include <myolinux/myoclient.h>
#include <myolinux/serial.h>
#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <math.h>

#include "libowinew.hpp"
#include "utils.hpp"
#include "myoband.hpp"
#include "owiarm.hpp"
#include "ecginterpreter.hpp"

using namespace std;
using namespace myolinux;
using namespace sf;


int main() {
    // #define CODE_WITH_KEYBOARD
    #define CODE_WITH_BASE

    #ifdef CODE_WITH_BASE
    OwiArm arm;
    MyoBand band;
    EcgInterpreter interpreter;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "yArm");
    Vec3f baseOrientation = toEulerAngle(band.getOrientation());
    bool pinceState = false;
    long long lastPinceMove = 0;
    arm.setOrientation(baseOrientation); // A l'initialisation, le bras est à la position du myo

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
                switch(event.key.code) {
                    case sf::Keyboard::Space:
                        arm.setOrientation((toEulerAngle(band.getOrientation())));
                        break;
                    case sf::Keyboard::P:
                        arm.openPince();
                        cout << "open pince" << endl;
                        break;
                    case sf::Keyboard::O:
                        arm.closePince();
                        cout << "open pince" << endl;
                        break;

                    case sf::Keyboard::Escape:
                        window.close();
                        break;

                    default:
                        break;
                }
        }
        double data[8];
        band.update();
        band.getEMGArray(data);
        interpreter.feedData(data);
        window.draw(interpreter);
        window.display();
        Vec4f orientation = band.getOrientation();
        Vec3f target = toEulerAngle(orientation);
        target.z = 0;
        arm.setTargetOrientation(target);
        if(interpreter.getPrediction() && secondsSince(lastPinceMove) > 1) {
            if(pinceState) {
                arm.openPince();
            } else {
                arm.closePince();
            }
            pinceState = !pinceState;
            lastPinceMove = getMicrotime();
        }
        arm.update();
    }

    #elif defined(CODE_WITH_KEYBOARD)
    OwiCommander commander;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "yArm");
    while (window.isOpen()) {
        sf::Event event;
        int cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0}; 
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
                switch(event.key.code) {

                    case sf::Keyboard::A:
                        cmd[0] = -1;
                        break;
                    case sf::Keyboard::Q:
                        cmd[0] = 1;
                        break;

                    case sf::Keyboard::Z:
                        cmd[1] = -1;
                        break;
                    case sf::Keyboard::S:
                        cmd[2] = 1;
                        break;

                    case sf::Keyboard::E:
                        cmd[3] = -1;
                        break;
                    case sf::Keyboard::D:
                        cmd[3] = 1;
                        break;

                    case sf::Keyboard::R:
                        cmd[4] = -1;
                        break;
                    case sf::Keyboard::F:
                        cmd[4] = 1;
                        break;

                    default:
                        break;
                }
        }
        for(unsigned int i = 0; i < 8; i++)
            cout << cmd[i];
        cout << endl;
        commander.setCMD(cmd);
        sleepMS(10);
    }

    #else


    OwiCommander commander;
    while(true) {
        //int cmd[8] = {0, 0, 0, -1, 0, 0, 0, 0}; // EPAULE
        int cmd[8] = {0, 1, 0, 0, 0, 0, 0, 0}; // COUDE
      //  int cmd[8] = {-1, 0, 0, 0, 0, 0, 0, 0}; // EPAULE
//        int cmd[8] = {0, 0, 1, 0, 0, 0, 0, 0}; // PINCE

        commander.setCMD(cmd);
        sleepMS(10);
    }
    #endif
}
