#ifndef OWIARM
#define OWIARM

#include <iostream>
#include "utils.hpp"
#include "libowi.hpp"

#define BASE_Y_ROT_SPEED 0.2f // rad/s
#define BASE_X_ROT_SPEED 0.2f

#define MASK_STOP_HAND 0x03 // 0000 0011
#define MASK_OPEN_HAND 0x02 // 0000 0010
#define MASK_CLOSE_HAND 0x01 // 0000 0001
#define MASK_STOP_WRIST 0x0C // 0000 1100 
#define MASK_WRIST_UP 0x04 // 0000 0100
#define MASK_WRIST_DOWN 0x08 // 0000 1000
#define MASK_STOP_ELBOW 0x30 // 0011 0000
#define MASK_ELBOW_UP 0x10 // 0001 0000
#define MASK_ELBOW_DOWN 0x20 // 0010 0000
#define MASK_STOP_SHOULDER 0xC0 // 1100 0000
#define MASK_SHOULDER_UP 0x80 // 0100 0000
#define MASK_SHOULDER_DOWN 0x40 // 1000 0000
#define MASK_STOP_BASE 0x03 // 0000 0011
#define MASK_BASE_CLOCKWISE 0x01 //  000 0001
#define MASK_BASE_COUNTER_CLOCKWISE 0x02 // 0000 0010
#define MASK_LED_ON 0x01 // 0000 0001
#define MASK_LED_OFF 0x01 // 0000 0001
#define MASK_CLEAR 0x00



class OwiArm {
public:
    OwiArm();
    void update();
    void setTargetOrientation(Vec3f orientation) { mBasePositionTarget = orientation; }
    friend std::ostream& operator<<(std::ostream& os, const OwiArm& arm);
private:
    void _updateTheoricalBasePos();
    void _updateCMD();
    double _getSecondsSinceLastUpdate();

    Vec3f mBasePositionTarget;
    Vec3f mBasePosition;
    long long mLastUpdateTimestamp;
    unsigned char mCurrentCMD[3];
};

#endif