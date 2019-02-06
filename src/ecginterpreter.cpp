#include "ecginterpreter.hpp"

using namespace sf;
using namespace std;

EcgInterpreter::EcgInterpreter() {
    TA_RetCode retCode;

    retCode = TA_Initialize( );

    if( retCode != TA_SUCCESS )
        printf( "Cannot initialize TA-Lib (%d)!\n", retCode );
    else  
         printf( "TA-Lib correctly initialized.\n" );
   

    for(unsigned int i = 0 ; i < 8 ; i++) {
        for(unsigned int j = 0 ; j < BUFFER_SIZE ; j++) {
            buffers[i][j] = sin(j / 10.0f) * 127.0f;
        }
        TA_Integer outBeg, outNbElement;
        retCode = TA_MA( 
            0,
            BUFFER_SIZE,
            &buffers[i][0],
            30,
            TA_MAType_SMA,
            &outBeg,
            &outNbElement,
            &mIndicators[i][0]
        );
    }
}

void EcgInterpreter::feedData(double data[8]) {
    for(unsigned int i = 0 ; i < 8 ; i++) {
        unsigned int j = int((getMicrotime() / 10e5f) * REFRESH_PERIOD) % BUFFER_SIZE;
        buffers[i][j] = data[i];
    }
}

void EcgInterpreter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(unsigned int i = 0 ; i < 8 ; i++) {
        VertexArray vertices(sf::Lines, BUFFER_SIZE * 2);
        vertices[0].position = Vector2f( 0, (i+1) * (SCREEN_HEIGHT / 8));
        vertices[BUFFER_SIZE * 2-1].position = Vector2f( SCREEN_WIDTH, (i+1) * (SCREEN_HEIGHT / 8));
        for(unsigned int j = 1 ; j < BUFFER_SIZE * 2 -1; j+=2) {
            vertices[j].position = Vector2f(
                j * (SCREEN_WIDTH / BUFFER_SIZE),
                (i+1) * (SCREEN_HEIGHT / 8)
                - abs(buffers[i][j/2] - (-127) * ((SCREEN_HEIGHT / 8) / abs(127-(-127))))
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
                j * (SCREEN_WIDTH / BUFFER_SIZE),
                (i+1) * (SCREEN_HEIGHT / 8)
                - abs(mIndicators[i][j/2] - (-127) * ((SCREEN_HEIGHT / 8) / abs(127-(-127))))
            );
            vertices[j].color = sf::Color::White;
            vertices[j+1] = vertices[j];
        }
        target.draw(vertices);
    }
}
