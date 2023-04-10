#ifndef __VEC4HPP__
#define __VEC4HPP__

#include "BASE.h"
#include "VEC3.hpp"
#include "VEC2.hpp"

template <typename FLOATTYPE>
struct __VEC4 {
private:
#define DVEC __VEC4

	using TYPE = FLOATTYPE;
	using TREFERENCE = TYPE&;
	using TCONSTREFERENCE = CONST TYPE&;

	using CTREF = TCONSTREFERENCE;
	using TREF = TREFERENCE;

	using VEC = DVEC<TYPE>;
	using REFERENCE = VEC&;
	using CONSTREFERENCE = CONST VEC&;

	using REF = REFERENCE;
	using CREF = CONSTREFERENCE;

public:
	TYPE x{}, y{}, z{}, w{};
	TREF r = x, g = y, b = z, a = w;

	DVEC() = default;
	DVEC(CTREF x, CTREF y, CTREF z, CTREF w): x(x), y(y), z(z), w(w) {}
	DVEC(CREF f, CREF s): x(s.x - f.x), y(s.y - f.y), z(s.z - f.z), w(s.w - f.w) {}
	DVEC(CONST __VEC3<TYPE>& vec): x(vec.x), y(vec.y), z(vec.z) {}
	DVEC(CONST __VEC2<TYPE>& vec): x(vec.x), y(vec.y) {}

	REF operator=(CREF other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	VEC operator-() CONST {
		return { -x, -y, -z, -w };
	}

	VEC operator+(CREF vec) CONST {
		return { -*THIS, vec };
	}

	VEC operator-(CREF vec) CONST {
		return { vec, *THIS };
	}

	VEC operator*(CTREF val) CONST {
		return { x * val, y * val, z * val, w * val };
	}

	VEC operator/(CTREF val) CONST {
		return { x / val, y / val, z / val, w / val };
	}

	VEC operator*(CREF vec) CONST {
		return { x * vec.x, y * vec.y, z * vec.z, w * vec.w };
	}

	VEC operator/(CREF vec) CONST {
		return { x / vec.x, y / vec.y, z / vec.z, w * vec.w };
	}

//UNAR AND BOOLEAN OPERATORS--------------------------------------
	REF operator+=(CREF vec) {
		return *THIS = (*THIS + vec);
	}

	REF operator-=(CREF vec) {
		return *THIS = (*THIS - vec);
	}

	REF operator*=(CTREF val) {
		return *THIS = (*THIS * val);
	}

	REF operator/=(CTREF val) {
		return *THIS = (*THIS / val);
	}

	REF operator*=(CREF vec) {
		return *THIS = (*THIS * vec);
	}

	REF operator/=(CREF vec) {
		return *THIS = (*THIS / vec);
	}

	bool operator==(CREF vec) CONST {
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}

	bool operator!=(CREF vec) CONST {
		return !(*THIS == vec);
	}
//----------------------------------------------------------------

	friend TYPE dt(CREF vec1, CREF vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
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
	operator DVEC<type>() CONST {
		return { x, y, z, w };
	}

private:
#undef DVEC
};

#endif