#ifndef __POLYGONHPP__
#define __POLYGONHPP__

#include "BASE.h"
#include "MATH.h"
#include <vector>

template <typename FLOATTYPE>
class __POLYGON : public std::vector<vec2<FLOATTYPE>> {
private:

	using TYPE = FLOATTYPE;
	using BASE = std::vector<vec2<TYPE>>;

	using TREFERENCE = TYPE&;
	using TCONSTREFERENCE = CONST TYPE&;

	using CTREF = TCONSTREFERENCE;
	using TREF = TREFERENCE;

	using VEC = vec2<TYPE>;
	using VREFERENCE = VEC&;
	using VCONSTREFERENCE = CONST VEC&;

	using VREF = VREFERENCE;
	using CVREF = VCONSTREFERENCE;

public:

	using BASE::BASE;

	VREF operator[](int i) {
		return BASE::data[i % BASE::size()];
	}

	CVREF operator[](int i) const {
		return BASE::data[i % BASE::size()];
	}

};

#endif