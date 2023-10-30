#ifndef __POLYGONHPP__
#define __POLYGONHPP__

#include "GEOMSTRUCTS.hpp"
#include "MATH.h"
#include <vector>
#include <algorithm>

template <typename FLOATTYPE>
class __POLYGON {
private:

	using TYPE = FLOATTYPE;

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


    std::vector <vec2 <FLOATTYPE>> data;

public:
    __POLYGON <FLOATTYPE>() {}
    __POLYGON <FLOATTYPE>(std::initializer_list <VEC> init) {
        data = init;
    }

    int size() const {
        return data.size();
    }

    CVREF operator[](int ind) const {
        return data[(ind % size() + size()) % size()];
    }

    VREF operator[](int ind) {
        return data[(ind % size() + size()) % size()];
    }

    CVREF get(int pos) const {
        return (*this)[pos];
    }

    VREF get(int pos) {
        return (*this)[pos];
    }

    void push(CTREF x, CTREF y) {
        data.emplace_back(x, y);
    }

    void push(const VEC v) {
        data.push_back(v);
    }

    bool empty() const {
        return data.empty();
    }

    decltype(auto) begin() {
        return data.begin();
    }
    decltype(auto) end() {
        return data.end();
    }
    decltype(auto) rbegin() {
        return data.begin();
    }
    decltype(auto) rend() {
        return data.end();
    }
    decltype(auto) cbegin() const {
        return data.cbegin();
    }
    decltype(auto) cend() const {
        return data.cend();
    }
    decltype(auto) crbegin() const {
        return data.cbegin();
    }
    decltype(auto) crend() const {
        return data.cend();
    }

    decltype(auto) erase(decltype(data.cbegin()) it) {
        return data.erase(it);
    }

    void pop() {
        data.pop_back();
    }

    friend bool isConvex(POLYGON p) {
        unsigned char flag = 0;
        for (size_t i = 0; i < p.size(); ++i) {
            auto cross = crss(p[i + 1] - p[i], p[i + 2] - p[i + 1]);
            if (cross < 0) {
                flag |= 1;
            }
            else if (cross > 0) {
                flag |= 2;
            }
        }
        return flag != 3;
    }

    friend bool contains(const POLYGON& a, const vec2f& p) {
        float angle = 0;
        for (int i = 0; i < a.size(); ++i) {
            angle += atan2(crss(a.get(i) - p, a.get(i + 1) - p), dt(a.get(i) - p, a.get(i + 1) - p));
        }
        return abs(angle) > acos(-1);
    }

    friend bool intersect(const POLYGON& a, const POLYGON& b) {
        for (size_t i = 0; i < a.size(); ++i) {
            if (contains(b, a.get(i))) {
                return true;
            }
        }
        for (size_t i = 0; i < b.size(); ++i) {
            if (contains(a, b.get(i))) {
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

    template <typename type>
    friend __POLYGON <type> convexHull(__POLYGON <type> p) {
        if (p.empty()) return {};

        std::sort(p.begin(), p.end(), [&](const vec2 <type>& a, const vec2 <type>& b) {
            if (a.x == b.x) return a.y < b.y;
            return a.x < b.x;
                  });
        std::sort(p.begin() + 1, p.end(), [&](const vec2 <type>& a, const vec2 <type>& b) {
            return cross(a - p[0], b - p[0]) > 0;
                  });

        __POLYGON <type> ans;
        for (auto v : p) {
            while (ans.size() > 1 && cross(ans[-1] - ans[-2], v - ans[-1]) <= 0) {
                ans.pop();
            }
            ans.push(v);
        }

        return ans;
    }

    void normalizeOrder() {
        if (empty()) return;

        std::sort(begin(), end(), [&](CVREF a, CVREF b) {
            if (a.x == b.x) return a.y < b.y;
            return a.x < b.x;
                  });
        std::sort(begin() + 1, end(), [&](CVREF a, CVREF b) {
            return cross(a - get(0), b - get(0)) > 0;
                  });
    }
};

using Polygonf = __POLYGON <FLOAT>;



bool convexContains(const Polygonf& poly, const vec2f& p) {
    if (poly.size() <= 4) return contains(poly, p);

    if (cross(poly[1] - poly[0], p - poly[0]) < 0 || cross(poly[-1] - poly[0], p - poly[0]) > 0) return false;

    int l = 1, r = poly.size();
    while (l + 1 != r) {
        int m = (l + r) / 2;
        if (cross(poly[m] - poly[0], p - poly[0]) < 0) r = m;
        else l = m;
    }

    return cross(poly[r] - poly[l], p - poly[l]) > 0;
}

Polygonf convexIntersecton(const Polygonf& p1, const Polygonf& p2) {
    std::vector <std::pair <vec2f, vec2f>> pls;
    for (int i = 0; i < p1.size(); ++i) {
        vec2f norm = p1.get(i + 1) - p1.get(i);
        pls.push_back({ p1.get(i), normalize(norm) });
    }
    for (int i = 0; i < p2.size(); ++i) {
        vec2f norm = p2.get(i + 1) - p2.get(i);
        pls.push_back({ p2.get(i), normalize(norm) });
    }

    sort(pls.begin(), pls.end(), [&](const std::pair <vec2f, vec2f>& a, const std::pair <vec2f, vec2f>& b) {
        if (a.second.y > 0 && b.second.y < 0) return true;
        if (a.second.y < 0 && b.second.y > 0) return false;
        if (a.second.y == 0 && b.second.y == 0) return a.second.x > b.second.x;
        return cross(a.second, b.second) > 0;
         });

    const double eps = 1e-5;

    auto inter = [&](const std::pair <vec2f, vec2f>& a, const std::pair <vec2f, vec2f>& b) {
        return a.first + a.second * (dot(a.second, b.first - a.first) + dot(a.second, b.second) * cross(a.second, b.first - a.first) / cross(b.second, a.second));
    };

    std::vector <int> st;
    for (int iter = 0; iter < 2; ++iter) {
        for (int i = 0; i < pls.size(); ++i) {
            auto [p, v] = pls[i];
            bool flag = true;
            while (!st.empty()) {
                if (length(v - pls[st.back()].second) < eps) {
                    if (cross(v, pls[st.back()].first - p) > 0) {
                        flag = false;
                        break;
                    }
                    else {
                        st.pop_back();
                    }
                }
                else if (cross(pls[st.back()].second, v) < eps / 2) {
                    return {};
                }
                else if (st.size() >= 2 && cross(pls[st[st.size() - 2]].second, inter(pls[st.back()], { p, v }) - pls[st[st.size() - 2]].first) < eps / 2) {
                    st.pop_back();
                }
                else {
                    break;
                }
            }
            if (flag) {
                st.push_back(i);
            }
        }
    }


    std::vector <int> cnt(pls.size(), 0);
    for (auto x : st) ++cnt[x];
    std::vector <std::pair <vec2f, vec2f>> pls1;
    for (int i = 0; i < pls.size(); ++i) {
        if (cnt[i] == 2) pls1.push_back(pls[i]);
    }
    Polygonf ans;
    for (int i = 0; i < pls1.size(); ++i) {
        ans.push(inter(pls1[i], pls1[(i + 1) % pls1.size()]));
    }

    return ans;
}

/// и какого хуя это в класс завернуто блять?
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
    static std::pair <int, int> getPointToConvexPolygonTangents(
        const vec_t& point,
        const POLYGON& polygon) {

        UINT lg = 0;
        while ((1 << lg) < polygon.size()) {
            ++lg;
        }

        int l1 = 0;
        for (int k = lg; k >= 0; --k) {
            vec_t v1 = polygon.get(l1 - (1 << k)) - point;
            vec_t v2 = polygon.get(l1) - point;
            vec_t v3 = polygon.get(l1 + (1 << k)) - point;
            if (cross(v1, v2) <= 0 && cross(v1, v3) <= 0) {
                l1 = l1 - (1 << k);
            }
            else if (cross(v3, v2) <= 0 && cross(v3, v1) <= 0) {
                l1 = l1 + (1 << k);
            }
            l1 = (l1 + polygon.size() * 228) % polygon.size();
        }

        int l2 = 0;
        for (int k = lg; k >= 0; --k) {
            vec_t v1 = polygon.get(l2 - (1 << k)) - point;
            vec_t v2 = polygon.get(l2) - point;
            vec_t v3 = polygon.get(l2 + (1 << k)) - point;
            if (cross(v1, v2) >= 0 && cross(v1, v3) >= 0) {
                l2 = l2 - (1 << k);
            }
            else if (cross(v3, v2) >= 0 && cross(v3, v1) >= 0) {
                l2 = l2 + (1 << k);
            }
            l2 = (l2 + polygon.size() * 228) % polygon.size();
        }

        return { l1, l2 };
    }

    static std::pair <int, int> getParTangents(
        const vec_t& dir,
        const POLYGON& polygon) {

        UINT lg = 0;
        while ((1 << lg) < polygon.size()) {
            ++lg;
        }

        int l1 = 0, l2 = 0;

        for (int k = lg; k >= 0; --k) {
            vec_t v1 = polygon.get(l1 - (1 << k));
            vec_t v2 = polygon.get(l1);
            vec_t v3 = polygon.get(l1 + (1 << k));
            if (cross(dir, v1) <= cross(dir, v2) && cross(dir, v1) <= cross(dir, v3)) {
                l1 = l1 - (1 << k);
            }
            else if (cross(dir, v3) <= cross(dir, v2) && cross(dir, v3) <= cross(dir, v1)) {
                l1 = l1 + (1 << k);
            }
            l1 = (l1 + polygon.size() * 228) % polygon.size();
        }
        for (int k = lg; k >= 0; --k) {
            vec_t v1 = polygon.get(l2 - (1 << k));
            vec_t v2 = polygon.get(l2);
            vec_t v3 = polygon.get(l2 + (1 << k));
            if (cross(dir, v1) >= cross(dir, v2) && cross(dir, v1) >= cross(dir, v3)) {
                l2 = l2 - (1 << k);
            }
            else if (cross(dir, v3) >= cross(dir, v2) && cross(dir, v3) >= cross(dir, v1)) {
                l2 = l2 + (1 << k);
            }
            l2 = (l2 + polygon.size() * 228) % polygon.size();
        }


        return { l1, l2 };
    }
};
/// сука для выпуклых только работает
std::pair <int, int> getParTangents(const vec2f& dir, const __POLYGON <float>& poly) {
    return TangentsFinder <float>::getParTangents(dir, poly);
}

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
        if (isConvex(polygon)) {
            return convexTriangulation(polygon);
        } 
        return nonConvexTriangulation(polygon);
    }
};

vec2f getCenter(const __POLYGON <float>& p) {
    /*for (auto pnt : p) {
        debug << pnt.x << ' ' << pnt.y << '\n';
    }
    debug << "-----------------------------------------\n";*/

    if (p.empty()) return vec2f(1337, 1488);
    if (p.size() == 1) return p[0];
    if (p.size() == 2) return (p[0] + p[1]) / 2;

    

    auto tr = PolygonTriangulator <float>::get(p);
    float sm = 0;
    vec2f cent = { 0, 0 };
    for (const auto& [a, b, c] : tr) {
        float ar = abs(cross(b - a, c - a));
        sm += ar;
        cent += (a + b + c) / 3 * ar;
    }
    return cent / sm;
}

#endif