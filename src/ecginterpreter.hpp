#ifndef ECG_INTERPRETER
#define ECG_INTERPRETER

#define SCREEN_WIDTH  1900
#define SCREEN_HEIGHT 1000

#define REFRESH_PERIOD (10 * 10e-3f) // 10ms
#define BUFFER_SIZE 1900

#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "ta-lib/ta_libc.h"
#include <math.h>
#include <iostream>

class EcgInterpreter : public sf::Drawable  {
    public:
    EcgInterpreter();
    void feedData(double data[8]);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
    double buffers[8][BUFFER_SIZE];
    double mIndicators[8][BUFFER_SIZE];
};

#endif