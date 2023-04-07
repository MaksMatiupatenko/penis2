#ifndef __VEC2HPP__
#define __VEC2HPP__

#include "BASE.h"

template <typename FLOATTYPE>
struct __VEC2 {
private:
#define DVEC __VEC2
	using TYPE = FLOATTYPE;
	using TREFERENCE = TYPE&;
	using TCONSTREFERENCE = CONST TREFERENCE;

	using CTREF = TCONSTREFERENCE;
	using TREF = TREFERENCE;
	
	using VEC = DVEC<TYPE>;
	using REFERENCE = VEC&;
	using CONSTREFERENCE = CONST REFERENCE;

	using REF = REFERENCE;
	using CREF = CONSTREFERENCE;

public:
	TYPE x{}, y{};
	TREF r = x, g = y;

	DVEC() = default;
	DVEC(CTREF x, CTREF y): x(x), y(y) {}
	DVEC(CREF f, CREF s): x(s.x - f.x), y(s.y - f.y) {}

	VEC operator-() CONST {
		return { -x, -y };
	}

	VEC operator+(CREF vec) CONST {
		return { -*THIS, vec };
	}

	VEC operator-(CREF vec) CONST {
		return { vec, *THIS };
	}

	VEC operator*(CTREF val) CONST {
		return { x * val, y * val };
	}

	VEC operator/(CTREF val) CONST {
		return { x / val, y / val };
	}

	VEC operator*(CREF vec) CONST {
		return { x * vec.x, y * vec.y };
	}

	VEC operator/(CREF vec) CONST {
		return { x / vec.x, y / vec.y };
	}

	friend TYPE dt(CREF vec1, CREF vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	friend TYPE crss(CREF vec1, CREF vec2) {
		return vec1.x * vec2.y - vec1.y * vec2.x;
	}

	friend TYPE lensq(CREF vec1) {
		return dt(vec1, vec1);
	}


	friend TYPE len(CREF vec1) {
		return sqrt(lenpw2(vec1));
	}

	friend TYPE dstsq(CREF vec1, CREF vec2) {
		return lensq({ vec1, vec2 });
	}

	friend TYPE dst(CREF vec1, CREF vec2) {
		return sqrt(dstpw2(vec1, vec2));
	}

	template <class type>
	operator type() CONST {
		return { x, y };
	}

private:
#undef DVEC
};

#endif