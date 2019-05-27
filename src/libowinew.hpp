#ifndef OWICOMMANDER
#define OWICOMMANDER

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


class OwiCommander {
public:
    OwiCommander();
    ~OwiCommander();
    void setCMD(int cmd[8]);
    bool getOrientationValues(double *x, double *z);
private:
    int mFileDescriptor;
    double mX;
    double mZ;
};

#endif
