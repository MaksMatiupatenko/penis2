#ifndef __BASEH__
#define __BASEH__

#include "GLLOADER.hpp"

std::ofstream debug("debug.txt");
std::ofstream errLog("runtimeLog.txt");

#define CONST const
#define VOID void
#define THIS this
#define lenpw2 lensq
#define dstpw2 dstsq
#define TRVWIDTH 16

typedef float FLOAT;
typedef int INT;
typedef long long INT64;
typedef double FLOAT64;
typedef unsigned int UINT;

#endif