#include "libowinew.hpp"
 #define DEBUG

using namespace std;

OwiCommander::OwiCommander() {
    mFileDescriptor = open("/dev/ttyACM1", O_RDWR);
    mX = 0;
    mZ = 0;
    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    // Read in existing settings, and handle any error
    if(tcgetattr(mFileDescriptor, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 1;

    // Set in/out baud rate to be 115200
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(mFileDescriptor, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
}

void OwiCommander::setCMD(int cmd[8]) {
    fsync(mFileDescriptor);

    // Write to serial port
    unsigned char msg[9];
    for(unsigned int i = 0; i < 8; i++) {
        switch(cmd[i]) {
            case -1:
                msg[i] = 'A';
                //printf("A");
                break;
            case 1:
                msg[i] = 'T';
            //    printf("T");
                break;
            case 0:
            //    printf("E");
                msg[i] = 'E';
                break;
        }
    }
    //printf("\n");
    msg[8] = '\n';
    write(mFileDescriptor, msg, sizeof(msg));
    fsync(mFileDescriptor);
    char read_buf[256];
    memset(&read_buf, '\0', sizeof(read_buf));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes = read(mFileDescriptor, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
    }

    char *token;

    char *rest = read_buf;

    while ((token = strtok_r(rest, "\n", &rest))) {
            char *token2;
            char *rest2 = token;
            int index = 0;
            while ((token2 = strtok_r(rest2, ",", &rest2))) {
                if(index == 0) {
                    mX = atof(token2) / 360 * (2 * 3.14);
                } else if (index == 1) {
                    mZ = -atof(token2) / 360 * (2 * 3.14);
                }
                index++;
            }
    }
}

bool OwiCommander::getOrientationValues(double* x, double* z) {
    *x = mX;
    *z = mZ;
    return true;
}


OwiCommander::~OwiCommander() {
    int msg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    setCMD(msg);
    printf("stop");
    close(mFileDescriptor);
}
