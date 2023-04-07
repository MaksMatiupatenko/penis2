#ifndef __MAT3HPP__
#define __MAT3HPP__

#include "BASE.h"
#include "VEC2.hpp"

template <typename FLOATTYPE>
class __MAT3 {
private:
#define DMAT __MAT3

	using TYPE = FLOATTYPE;
	using TREFERENCE = TYPE&;
	using TCONSTREFERENCE = CONST TYPE&;
	using TPTR = TYPE*;
	using CTPTR = CONST TYPE*;

	using CTREF = TCONSTREFERENCE;
	using TREF = TREFERENCE;

	using MAT = DMAT<TYPE>;
	using REFERENCE = MAT&;
	using CONSTREFERENCE = CONST MAT&;

	using REF = REFERENCE;
	using CREF = CONSTREFERENCE;

public:
	TYPE data[3][3] = { {(TYPE)1, (TYPE)0, (TYPE)0}, {(TYPE)0, (TYPE)1, (TYPE)0}, {(TYPE)0, (TYPE)0, (TYPE)1} };
	DMAT(CTREF n00, CTREF n01, CTREF n02, CTREF n10, CTREF n11, CTREF n12, CTREF n20, CTREF n21, CTREF n22) {
		data[0][0] = n00;
		data[0][1] = n01;
		data[0][2] = n02;
		data[1][0] = n10;
		data[1][1] = n11;
		data[1][2] = n12;
		data[2][0] = n20;
		data[2][1] = n21;
		data[2][2] = n22;
	}

	TPTR operator[](INT ind) {
		return data[ind];
	}

	CTPTR operator[](INT ind) CONST {
		return data[ind];
	}

	MAT operator*(CREF mat) {
		return {
			data[0][0] * mat[0][0] + data[0][1] * mat[1][0] + data[0][2] * mat[2][0],
			data[0][0] * mat[0][1] + data[0][1] * mat[1][1] + data[0][2] * mat[2][1],
			data[0][0] * mat[0][2] + data[0][1] * mat[1][2] + data[0][2] * mat[2][2],
			data[1][0] * mat[0][0] + data[1][1] * mat[1][0] + data[1][2] * mat[2][0],
			data[1][0] * mat[0][1] + data[1][1] * mat[1][1] + data[1][2] * mat[2][1],
			data[1][0] * mat[0][2] + data[1][1] * mat[1][2] + data[1][2] * mat[2][2],
			data[2][0] * mat[0][0] + data[2][1] * mat[1][0] + data[2][2] * mat[2][0],
			data[2][0] * mat[0][1] + data[2][1] * mat[1][1] + data[2][2] * mat[2][1],
			data[2][0] * mat[0][2] + data[2][1] * mat[1][2] + data[2][2] * mat[2][2]
		};
	}
};


__MAT3<FLOAT> translate(FLOAT x, FLOAT y) {
	return __MAT3<FLOAT>(
		1, 0, x,
		0, 1, y,
		0, 0, 1
		);
}
__MAT3<FLOAT> translate(__VEC2 <FLOAT> vec) {
	return translate(vec.x, vec.y);
}

__MAT3<FLOAT> rotate(FLOAT angle) {
	return __MAT3<FLOAT>(
		cos(angle), sin(angle), 0,
		-sin(angle), cos(angle), 0,
		0, 0, 1
		);
}


#endif