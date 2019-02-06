#ifndef ECG_INTERPRETER
#define ECG_INTERPRETER

#define SCREEN_WIDTH  1900
#define SCREEN_HEIGHT 1000

#define REFRESH_PERIOD 0.01f // 100HZ
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
    void _computeIndicators();
    void _updatePrediction();
    double mBuffers[8][BUFFER_SIZE];
    double mShiftedIndicators[8][BUFFER_SIZE];  // We're keeping them in memory just to avoid continously pushing to the stack
    double mIndicators[8][BUFFER_SIZE]; 
    double mThreshold;
    bool   mPrediction[8];
    unsigned int mDataIndex;
    clock_t mFrameStart;
};

#endif