#ifndef __POLYGONHPP__
#define __POLYGONHPP__

#include "GEOMSTRUCTS.hpp"
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

    using POLYGON = __POLYGON<TYPE>;

public:

	using BASE::BASE;

    CVREF get(int pos) const {
        return BASE::operator[]((pos % (int)BASE::size() + (int)BASE::size()) % (int)BASE::size());
    }

    VREF get(int pos) {
        return BASE::operator[]((pos % (int)BASE::size() + (int)BASE::size()) % (int)BASE::size());
    }

    void push(CTREF x, CTREF y) {
        BASE::emplace_back(x, y);
    }

    static bool isConvex(POLYGON polygon) {
        unsigned char flag = 0;
        for (size_t i = 0; i < polygon.size(); ++i) {
            TYPE cross = crss(VEC(polygon.get(i), polygon.get(i + 1)), VEC(polygon.get(i + 1), polygon.get(i + 2)));
            if (cross < 0) {
                flag |= 1;
            }
            else if (cross > 0) {
                flag |= 2;
            }
        }
        return flag != 3;
    }

    static bool contains(const POLYGON& a, const vec2f& p) {
        float angle = 0;
        for (int i = 0; i < a.size(); ++i) {
            angle += atan2(crss(a.get(i) - p, a.get(i + 1) - p), dt(a.get(i) - p, a.get(i + 1) - p));
        }
        return abs(angle) > acos(-1);
    }

    static vec2f getNormal(const POLYGON& a, const vec2f& p) {
        float adst = 1e10;
        int ind = 0;
        for (int i = 0; i < a.size(); ++i) {
            if (segmentDist(a.get(i + 1), a.get(i), p) < adst) {
                adst = segmentDist(a.get(i + 1), a.get(i), p);
                ind = i;
            }
        }

        return normalize(vec2f((a.get(ind + 1) - a.get(ind)).y, -(a.get(ind + 1) - a.get(ind)).x));
    }

    static vec2f getCollNormal(const POLYGON& a, const POLYGON& b) {
        for (size_t i = 0; i < a.size(); ++i) {
            if (POLYGON::contains(b, a.get(i))) {
                return getNormal(b, a.get(i));
            }
        }
        for (size_t i = 0; i < b.size(); ++i) {
            if (POLYGON::contains(a, b.get(i))) {
                return getNormal(a, b.get(i));
            }
        }
        return { 0, 0 };
    }

    static bool intersect(const POLYGON& a, const POLYGON& b) {
        for (size_t i = 0; i < a.size(); ++i) {
            if (POLYGON::contains(b, a.get(i))) {
                return true;
            }
        }
        for (size_t i = 0; i < b.size(); ++i) {
            if (POLYGON::contains(a, b.get(i))) {
                return true;
            }
        }
        for (size_t i = 0; i < a.size(); ++i) {
            Segment seg1(a.get(i), a.get(i + 1));
            for (size_t j = 0; j < b.size(); ++j) {
                if (intersectg(seg1, Segment(b.get(j), b.get(j + 1)))) {
                    return true;
                }
            }
        }
        return false;
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

        throw emptyRealisation;

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

template <typename FLOATTYPE>
class PolygonTriangulator {
private:
    using TYPE = FLOATTYPE;
    using vec_t = vec2<TYPE>;
    using POLYGON = __POLYGON<TYPE>;

    static std::vector <Triangle> nonConvexTriangulation(const POLYGON& polygon) {
        auto p = polygon;
        std::vector <Triangle> ans;

        int i = 0;
        while (p.size() > 2) {
            if (crss(p.get(i) - p.get(i - 1), p.get(i + 1) - p.get(i - 1)) >= 0) {
                bool flag = true;
                for (int j = 0; j < p.size() - 3; ++j) {
                    if (
                        crss(p.get(i) - p.get(i - 1), p.get(i + 2 + j) - p.get(i - 1)) >= 0 &&
                        crss(p.get(i + 1) - p.get(i), p.get(i + 2 + j) - p.get(i)) >= 0 &&
                        crss(p.get(i - 1) - p.get(i + 1), p.get(i + 2 + j) - p.get(i + 1)) >= 0) {

                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    ans.push_back({ p.get(i - 1), p.get(i), p.get(i + 1) });
                    p.erase(p.begin() + i % p.size());
                    --i;
                }
            }
            ++i;
        }

        return ans;
    }

    static std::vector <Triangle> convexTriangulation(const POLYGON& polygon) {
        std::vector <Triangle> result;
        for (size_t i = 2; i < polygon.size(); ++i) {
            result.emplace_back(polygon[0], polygon[i - 1], polygon[i]);
        }
        return result;
    }

public:
    static std::vector <Triangle> get(const POLYGON& polygon) {
        if (POLYGON::isConvex(polygon)) {
            return convexTriangulation(polygon);
        } 
        return nonConvexTriangulation(polygon);
    }
};



#endif