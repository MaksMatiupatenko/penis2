#ifndef __TRIANGLEHPP__
#define __TRIANGLEHPP__

#include "MATH.h"

struct Triangle {
    vec2f a{}, b{}, c{};
    Triangle() = default;
    Triangle(vec2f a, vec2f b, vec2f c) :
        a(a), b(b), c(c) { }
};

#endif // !__TRIANGLEHPP__
