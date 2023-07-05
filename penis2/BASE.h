#ifndef __BASEH__
#define __BASEH__

#include "GLLOADER.hpp"
#include <windowsx.h>
#include <random>
#include <chrono>

std::ofstream debug("debug.txt");
std::ofstream errLog("runtimeLog.txt");
std::exception emptyRealisation("Suka resalisuy snachala potom usay dolbaeb\n");

const FLOAT PI = acosf(-1);

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

#undef max
std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
double rnd01() {
	return (double)rnd() / rnd.max();
}
#define max(a, b) (((a) > (b)) ? (a) : (b))

#endif