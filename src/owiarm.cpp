#include "owiarm.hpp"

using namespace std;

OwiArm::OwiArm() {
    for(unsigned int i = 0; i < 8; i++) {
        mCurrentCMD[i] = 0;
    }
    mBasePosition = {0,0,0};
    mLastUpdateTimestamp = getMicrotime();
}

void OwiArm::_updateTheoricalBasePos() {
    if(mCurrentCMD[0] == -1) {
        mBasePosition.x += _getSecondsSinceLastUpdate() * BASE_X_ROT_SPEED;
    }
    if(mCurrentCMD[0] == 1) {
        mBasePosition.x -= _getSecondsSinceLastUpdate() * BASE_X_ROT_SPEED;
    }
    if(mCurrentCMD[1] == -1) {
        //mBasePosition.y += _getSecondsSinceLastUpdate() * BASE_Y_ROT_SPEED / 1.5;
    }
    if(mCurrentCMD[1] == 1) {
        //mBasePosition.y -= _getSecondsSinceLastUpdate() * BASE_Y_ROT_SPEED / 2.5;
    }
    if(mCurrentCMD[3] == -1) {
        // mBasePosition.z += _getSecondsSinceLastUpdate() * BASE_Z_ROT_SPEED;
    }
    if(mCurrentCMD[3] == 1) {
        // mBasePosition.z -= _getSecondsSinceLastUpdate() * BASE_Z_ROT_SPEED;
    }

    mLastUpdateTimestamp = getMicrotime();
}

void OwiArm::_updateCMD() {
    if(mBasePosition.x < mBasePositionTarget.x) {
        mCurrentCMD[0] = -1;
    } else {
        mCurrentCMD[0] = 1;
    }
    if(abs(mBasePosition.x - mBasePositionTarget.x) < 0.05) {
        mCurrentCMD[0] = 0;
    }
   if(mBasePosition.y < mBasePositionTarget.y) {
        mCurrentCMD[1] = -1;
    } else {
        mCurrentCMD[1] = 1;
    }
    if(abs(mBasePosition.y - mBasePositionTarget.y) < 0.05) {
        mCurrentCMD[1] = 0;
    }
    if(mBasePosition.z < mBasePositionTarget.z) {
        mCurrentCMD[3] = -1;
    } else {
        mCurrentCMD[3] = 1;
    }
    if(abs(mBasePosition.z - mBasePositionTarget.z) < 0.05) {
        mCurrentCMD[3] = 0;
    }

    if(mCurrentCMD[2] == -1 && _getSecondsSinceLastPinceMove() > 1.3) {
        mCurrentCMD[2] = 0;
    }
    if(mCurrentCMD[2] == 1 && _getSecondsSinceLastPinceMove() > 1.5) {
        mCurrentCMD[2] = 0;
    }
    mCommander.setCMD(mCurrentCMD);
    mCommander.getOrientationValues(&mBasePosition.y, &mBasePosition.z);
    //cout << "y: " << mBasePosition.y << "\ty: " << mBasePosition.z << endl;
}

double OwiArm::_getSecondsSinceLastUpdate() {
    return (getMicrotime() - mLastUpdateTimestamp) / 10e5f;
}

double OwiArm::_getSecondsSinceLastPinceMove() {
    return (getMicrotime() - mLastPinceMoveTimestamp) / 10e5f;
}



void OwiArm::update() {
    _updateTheoricalBasePos();
    _updateCMD();
}

void OwiArm::openPince() {
    mCurrentCMD[2] = -1;
    mLastPinceMoveTimestamp = getMicrotime();
}

void OwiArm::closePince() {
    mCurrentCMD[2] = 1;
    mLastPinceMoveTimestamp = getMicrotime();
}

ostream& operator<<(ostream& os, const OwiArm& arm)
{
   os << arm.mBasePosition.x << "\t" << arm.mBasePosition.y;
   return os;
}
