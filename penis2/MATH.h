#ifndef __MATHH__
#define __MATHH__

#include "BASE.h";

//--------------------------------------------------- 2 dimensional vector
#include "VEC2.hpp"

template <typename FLOATTYPE>
using vec2 = __VEC2<FLOATTYPE>;

///Undefined behaviour warning.
///Integer multiplication overflow to be expected.
///Consider go fuck yourself.
using vec2i = vec2<INT>;

///Undefined behaviour warning.
///Long long integer multiplication overflow to be expected.
///Consider go fuck yourself.
using vec2l = vec2<INT64>;

using vec2f = vec2<FLOAT>;
using vec2d = vec2<FLOAT64>;
//-------------------------------------------------------------------------


//--------------------------------------------------- 3 dimensional vector
#include "VEC3.hpp"

template <typename FLOATTYPE>
using vec3 = __VEC3<FLOATTYPE>;

///Undefined behaviour warning.
///Integer multiplication overflow to be expected.
///Consider go fuck yourself.
using vec3i = vec3<INT>;

///Undefined behaviour warning.
///Long long integer multiplication overflow to be expected.
///Consider go fuck yourself.
using vec3l = vec3<INT64>;

using vec3f = vec3<FLOAT>;
using vec3d = vec3<FLOAT64>;
//-------------------------------------------------------------------------


//--------------------------------------------------- 4 dimensional vector
#include "VEC4.hpp"

template <typename FLOATTYPE>
using vec4 = __VEC4<FLOATTYPE>;

///Undefined behaviour warning.
///Integer multiplication overflow to be expected.
///Consider go fuck yourself.
using vec4i = vec4<INT>;

///Undefined behaviour warning.
///Long long integer multiplication overflow to be expected.
///Consider go fuck yourself.
using vec4l = vec4<INT64>;

using vec4f = vec4<FLOAT>;
using vec4d = vec4<FLOAT64>;
//-------------------------------------------------------------------------


//--------------------------------------------------- 3x3 dimensional matrix
#include "MAT3.hpp"

template <typename FLOATTYPE>
using mat3 = __MAT3<FLOATTYPE>;

///Undefined behaviour warning.
///Integer multiplication overflow to be expected.
///Consider go fuck yourself.
using mat3i = mat3<INT>;

///Undefined behaviour warning.
///Long long integer multiplication overflow to be expected.
///Consider go fuck yourself.
using mat3l = mat3<INT64>;

using mat3f = mat3<FLOAT>;
using mat3d = mat3<FLOAT64>;
//-------------------------------------------------------------------------


#endif
