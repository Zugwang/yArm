#ifndef OWICOMMANDER
#define OWICOMMANDER

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>

class OwiCommander {
public:
    OwiCommander();
    ~OwiCommander();
    void setCMD(int cmd[8]);
private:
    int mFileDescriptor;
};

#endif
