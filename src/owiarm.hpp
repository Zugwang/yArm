#ifndef OWIARM
#define OWIARM

#include <iostream>
#include "libowinew.hpp"
#include "utils.hpp"

#define BASE_Y_ROT_SPEED 0.2f // rad/s
#define BASE_X_ROT_SPEED 0.2f



class OwiArm {
public:
    OwiArm();
    void update();
    void setTargetOrientation(Vec3f orientation) { mBasePositionTarget = orientation; }
    void setOrientation(Vec3f orientation) { mBasePosition = orientation; }
    void closePince();
    void openPince();
    friend std::ostream& operator<<(std::ostream& os, const OwiArm& arm);
private:
    void _updateTheoricalBasePos();
    void _updateCMD();
    double _getSecondsSinceLastUpdate();
    double _getSecondsSinceLastPinceMove();

    Vec3f mBasePositionTarget;
    Vec3f mBasePosition;
    OwiCommander mCommander;
    long long mLastUpdateTimestamp;
    long long mLastPinceMoveTimestamp;
    int mCurrentCMD[8];
};

#endif
