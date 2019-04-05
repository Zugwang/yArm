#ifndef ECG_INTERPRETER
#define ECG_INTERPRETER

#define SCREEN_WIDTH  1600.0f
#define SCREEN_HEIGHT 600.0f

#define REFRESH_PERIOD 0.005f // 100HZ
#define BUFFER_SIZE 1600

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
    int getPrediction() {
        int val = 0;
        for(unsigned int i = 0; i < 8; i++)
            val += mPrediction[i];
        return val;
    }

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
