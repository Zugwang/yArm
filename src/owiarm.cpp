#include "owiarm.hpp"

using namespace std;

OwiArm::OwiArm() {
    owi_init();
    mBasePosition = {0,0,0};
    mLastUpdateTimestamp = getMicrotime();
}

void OwiArm::_updateTheoricalBasePos() {
    if(mCurrentCMD[1] & MASK_BASE_CLOCKWISE) {
        mBasePosition.x -= _getSecondsSinceLastUpdate() * BASE_X_ROT_SPEED;
    }
    if(mCurrentCMD[1] & MASK_BASE_COUNTER_CLOCKWISE) {
        mBasePosition.x += _getSecondsSinceLastUpdate() * BASE_X_ROT_SPEED;
    }
    if(mCurrentCMD[0] & MASK_SHOULDER_UP) {
        mBasePosition.y += _getSecondsSinceLastUpdate() * BASE_Y_ROT_SPEED;
    }
    if(mCurrentCMD[0] & MASK_SHOULDER_DOWN) {
        mBasePosition.y -= _getSecondsSinceLastUpdate() * BASE_Y_ROT_SPEED;
    }
    mLastUpdateTimestamp = getMicrotime();
}

void OwiArm::_updateCMD() {
    if(mBasePosition.x < mBasePositionTarget.x) {
        mCurrentCMD[1] &= ~MASK_STOP_BASE;
        mCurrentCMD[1] |= MASK_BASE_COUNTER_CLOCKWISE;
    } else {
        mCurrentCMD[1] &= ~MASK_STOP_BASE;
        mCurrentCMD[1] |= MASK_BASE_CLOCKWISE;
    }
    if(mBasePosition.y < mBasePositionTarget.y) {
        mCurrentCMD[0] &= ~MASK_STOP_SHOULDER;
        mCurrentCMD[0] |= MASK_SHOULDER_UP;
    } else {
        mCurrentCMD[0] &= ~MASK_STOP_SHOULDER;
        mCurrentCMD[0] |= MASK_SHOULDER_DOWN;
    }
    owi_send_command(mCurrentCMD[0], mCurrentCMD[1], mCurrentCMD[2]);
}

double OwiArm::_getSecondsSinceLastUpdate() {
    return (getMicrotime() - mLastUpdateTimestamp) / 10e5f;
}

void OwiArm::update() {
    _updateTheoricalBasePos();
    _updateCMD();
}


ostream& operator<<(ostream& os, const OwiArm& arm)
{

    os << "x: " << arm.mBasePosition.x << "\ty: " << arm.mBasePosition.y;
    return os;
}