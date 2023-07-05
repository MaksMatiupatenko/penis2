#ifndef __VEC2HPP__
#define __VEC2HPP__

#include "BASE.h"

template <typename FLOATTYPE>
struct __VEC2 {
private:
#define DVEC __VEC2

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
	TYPE x{}, y{};
	TREF r = x, g = y;

	DVEC() = default;
	DVEC(CTREF x, CTREF y) : x(x), y(y) {}
	DVEC(CREF f, CREF s) : x(s.x - f.x), y(s.y - f.y) {}
	DVEC(CREF f) = default;

	REF operator=(CREF other) {
		x = other.x;
		y = other.y;
		return *this;
	}

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
		return x == vec.x && y == vec.y;
	}

	bool operator!=(CREF vec) CONST {
		return !(*THIS == vec);
	}

	TYPE lensq() const {
		return dt(*this, *this);
	}
//----------------------------------------------------------------

	friend TYPE lensq(CREF);
	

	friend TYPE dt(CREF vec1, CREF vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	friend TYPE dot(CREF vec1, CREF vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	friend TYPE crss(CREF vec1, CREF vec2) {
		return vec1.x * vec2.y - vec1.y * vec2.x;
	}

	friend TYPE cross(CREF vec1, CREF vec2) {
		return vec1.x * vec2.y - vec1.y * vec2.x;
	}

	friend TYPE lensq(CREF vec1) {
		return vec1.lensq();
	}

	friend TYPE len(CREF vec1) {
		return sqrt(vec1.lensq());
	}

	friend TYPE lenght(CREF vec1) {
		return sqrt(vec1.lensq());
	}

	friend TYPE length(CREF vec1) {
		return sqrt(vec1.lensq());
	}

	friend TYPE dstsq(CREF vec1, CREF vec2) {
		return VEC(vec1, vec2).lensq();
	}

	friend TYPE dst(CREF vec1, CREF vec2) {
		return sqrt(dstpw2(vec1, vec2));
	}

	friend VEC normalize(CREF vec) {
		if (vec.lensq() == 0) {
			return vec;
		}
		return vec / len(vec);
	}

	template <class type>
	operator DVEC<type>() CONST {
		return { x, y };
	}

private:
#undef DVEC
};


#endif