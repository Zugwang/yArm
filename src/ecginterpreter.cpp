#include "ecginterpreter.hpp"

using namespace sf;
using namespace std;

EcgInterpreter::EcgInterpreter() {
    mThreshold = 30;
    TA_RetCode retCode;

    retCode = TA_Initialize( );

    if( retCode != TA_SUCCESS )
        printf( "Cannot initialize TA-Lib (%d)!\n", retCode );
    else
         printf( "TA-Lib correctly initialized.\n" );
    mFrameStart = clock();
}

void EcgInterpreter::feedData(double data[8]) {
    double maxTime = BUFFER_SIZE * REFRESH_PERIOD;
    double sinceFrameStart  = (clock() - mFrameStart) / 1000.0f;
    unsigned int index = (unsigned int)((sinceFrameStart / maxTime)) % BUFFER_SIZE;
    for(unsigned int i = 0 ; i < 8 ; i++) {
        mBuffers[i][index] = data[i];
    }
    mDataIndex = index;
    _computeIndicators();
    _updatePrediction();
}

void EcgInterpreter::_computeIndicators() {
    for(unsigned int i = 0 ; i < 8 ; i++) {
        TA_Integer outBeg, outNbElement;
        TA_NATR(
            0,
            mDataIndex,
            &mBuffers[i][0],
            &mBuffers[i][0],
            &mBuffers[i][0],
            20,
            &outBeg,
            &outNbElement,
            &mShiftedIndicators[i][0]
        );
        mIndicators[i][mDataIndex] = mShiftedIndicators[i][mDataIndex-outBeg] / 100;
    }
}

void EcgInterpreter::_updatePrediction() {
    for(unsigned int i = 0 ; i < 8 ; i++) {
//        cout << mIndicators[i][mDataIndex] << "\t";
        mPrediction[i] = abs(mIndicators[i][mDataIndex]) > mThreshold;
    }
}

void EcgInterpreter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    RectangleShape rectangle(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT / 8));
//    cout << "mPrediction : ";
    for(unsigned int i = 0 ; i < 8 ; i++) {
   //     cout << mPrediction[i] << "\t";
        rectangle.setFillColor(mPrediction[i] ? Color::Blue : Color::Black);
        rectangle.setPosition(Vector2f(0,(i+1) * (SCREEN_HEIGHT / 8)));
        target.draw(rectangle);
    }
   // cout << endl;

    for(unsigned int i = 0 ; i < 8 ; i++) {
        VertexArray vertices(sf::Lines, BUFFER_SIZE * 2);
        vertices[0].position = Vector2f( 0, (i+1) * (SCREEN_HEIGHT / 8));
        vertices[BUFFER_SIZE * 2-1].position = Vector2f( SCREEN_WIDTH, (i+1) * (SCREEN_HEIGHT / 8));
        for(unsigned int j = 1 ; j < BUFFER_SIZE * 2 -1; j+=2) {
            vertices[j].position = Vector2f(
                (j/2) * (SCREEN_WIDTH / BUFFER_SIZE),
                (i+1) * (SCREEN_HEIGHT / 8)
                - abs(mBuffers[i][j/2] - (-127) * ((SCREEN_HEIGHT / 8) / abs(127-(-127))))
            );
            vertices[j].color = sf::Color::Red;
            vertices[j+1] = vertices[j];
        }
        target.draw(vertices);
    }
    for(unsigned int i = 0 ; i < 8 ; i++) {
        VertexArray vertices(sf::Lines, BUFFER_SIZE * 2);
        vertices[0].position = Vector2f( 0, (i+1) * (SCREEN_HEIGHT / 8));
        vertices[BUFFER_SIZE * 2-1].position = Vector2f( SCREEN_WIDTH, (i+1) * (SCREEN_HEIGHT / 8));
        for(unsigned int j = 1 ; j < BUFFER_SIZE * 2 -1; j+=2) {
            vertices[j].position = Vector2f(
                (j/2) * (SCREEN_WIDTH / BUFFER_SIZE),
                (i+1) * (SCREEN_HEIGHT / 8)
                - abs(mIndicators[i][j/2] - (-127) * ((SCREEN_HEIGHT / 8) / abs(127-(-127))))
            );
            vertices[j].color = sf::Color::White;
            vertices[j+1] = vertices[j];
        }
        target.draw(vertices);
    }
}
