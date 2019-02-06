#ifndef UTILS
#define UTILS

#include <unistd.h>
#include <sys/time.h>
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

struct Vec3f {
    double x;
    double y;
    double z;
};

struct Vec4f {
    double x;
    double y;
    double z;
    double w;
};

// Renvoie des millisecondes
long long getMicrotime();
// Convertit des quaternions en angles d'euler
Vec3f toEulerAngle(Vec4f q);
void sleepMS(int milliseconds); // cross-platform sleep function


#endif