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

template <typename FLOATTYPE>
class TangentsFinder {
private:
    using TYPE = FLOATTYPE;
    using vec_t = vec2<TYPE>;
    using POLYGON = __POLYGON<TYPE>;

public:
    /// <summary>
    /// Finds tanget from a point to a convex polygon in O(log2n) time and O(1) memory complexitys.
    /// </summary>
    /// <param name="point"></param>
    /// <param name="polygon"></param>
    /// <returns></returns>
    static std::pair<vec_t, vec_t> getPointToConvexPolygonTangents(
        const vec_t& point,
        const POLYGON& polygon) {

        UINT lg = 0;
        while ((1 << lg) < polygon.size()) {
            ++lg;
        }

        int l1 = 0;
        for (int k = lg; k >= 0; --k) {
            vec_t v1 = polygon[l1 - (1 << k)] - point;
            vec_t v2 = polygon[l1] - point;
            vec_t v3 = polygon[l1 + (1 << k)] - point;
            if (cross(v1, v2) < 0 && cross(v1, v3) < 0) {
                l1 = l1 - (1 << k);
            }
            else if (cross(v3, v2) < 0 && cross(v3, v1) < 0) {
                l1 = l1 + (1 << k);
            }
            l1 %= polygon.size();
        }

        int l2 = 0;
        for (int k = lg; k >= 0; --k) {
            vec_t v1 = polygon[l2 - (1 << k)] - point;
            vec_t v2 = polygon[l2] - point;
            vec_t v3 = polygon[l2 + (1 << k)] - point;
            if (cross(v1, v2) > 0 && cross(v1, v3) > 0) {
                l2 = l2 - (1 << k);
            }
            else if (cross(v3, v2) > 0 && cross(v3, v1) > 0) {
                l2 = l2 + (1 << k);
            }
            l2 %= polygon.size();
        }

        return { l1, l2 };
    }
};

#endif