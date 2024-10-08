#ifndef SURFACES_H
#define SURFACES_H

#include "real.h"

#include <cmath>
#include <iostream>
#include <functional>

struct Point {
    const Real x;
    const Real y;

    Point() = delete;
    Point(Real _x, Real _y) : x(_x), y(_y) {}
};

inline std::ostream &operator<<(std::ostream &os, const Point &p) {
    return os << p.x << ' ' << p.y;
}

using Surface = std::function<Real(Point)>;

// Generuje pĹaszczyznÄ f(x, y) = 0.
inline Surface plain() {
    return [](Point) -> Real {
        return 0.0;
    };
}


// Generuje powierzchniÄ f(x, y) = x.
inline Surface slope() {
    return [](Point p) {
        return p.x;
    };
}

// Generuje powierzchniÄ schodkowÄ wzdĹuĹź osi X o szerokoĹci schodka s
// (domyĹlnie 1) i wysokoĹci schodka 1. Dla 0 <= x < s mamy f(x, y) = 0,
// dla -s <= x < 0 mamy f(x, y) = -1 itd. JeĹli s <= 0, to f(x, y) = 0.
inline Surface steps(Real s = 1) {
    return [s](Point p) {
        const auto step_height = floor(p.x / s);
        return s <= 0 ? Real(0) : step_height;
    };
}

// Generuje powierzchniÄ, ktĂłra jest szachownicÄ o szerokoĹci kratki s
// (domyĹlnie 1). Dla 0 <= x < s mamy f(x, y) = 1, gdy 0 <= y < s
// oraz f(x, y) = 0, gdy s <= y < 2 * s itd. JeĹli s <= 0, to f(x, y) = 0.
inline Surface checker(Real s = 1) {
    return [s](Point p) {
        auto checker_sign = [s](Point p) {
            const int8_t sgn_x = std::abs(std::fmod(floor(p.x / s), 2.0)) ? 1 : -1;
            const int8_t sgn_y = std::abs(std::fmod(floor(p.y / s), 2.0)) ? 1 : -1;

            return sgn_x * sgn_y;
        };

        return s <= 0
               ? Real(0)
               : checker_sign(p) == 1;
    };
}


// Generuje powierzchniÄ f(x, y) = x * x.
inline Surface sqr() {
    return [](Point p) {
        return p.x * p.x;
    };
}

// Generuje powierzchniÄ sinusoidalnÄ wzglÄdem argumentu x.
inline Surface sin_wave() {
    return [](Point p) {
        return sin(p.x);
    };
}

// Generuje powierzchniÄ kosinusoidalnÄ wzglÄdem argumentu x.
inline Surface cos_wave() {
    return [](Point p) {
        return cos(p.x);
    };
}

// Generuje powierzchniÄ z koncentrycznymi naprzemiennymi pasami szerokoĹci s
// (domyĹlnie 1) o Ĺrodku w punkcie (0, 0), gdzie f(x, y) = 1 w najbardziej
// wewnÄtrznym okrÄgu (ĹÄcznie z tym okrÄgiem). JeĹli s <= 0, to f(x, y) = 0.
inline Surface rings(Real s = 1) {
    return [s](Point p) {
        auto inside_stripe = [s= s](Point p) {
            const Real dist = sqrt(p.x * p.x + p.y * p.y);

            return p.x == 0 and p.y == 0
                ? 1
                : std::abs(std::fmod(ceil(dist / s), 2.0));
        };

        return s <= 0 ? 0 : inside_stripe(p);
    };
}

// Generuje powierzchniÄ z elipsÄ o Ĺrodku w punkcie (0, 0) o pĂłĹosi dĹugoĹci
// a wzdĹuĹź osi X (domyĹlnie 1) i o pĂłĹosi dĹugoĹci b wzdĹuĹź osi Y
// (domyĹlnie 1). Zachodzi f(x, y) = 1, gdy (x, y) leĹźy wewnÄtrz lub na brzegu
// elipsy, zaĹ f(x, y) = 0, w. p.p. JeĹli a <= 0 lub b <= 0, to f(x, y) = 0.
inline Surface ellipse(Real a = 1, Real b = 1) {
    return [a, b](Point p)-> Real {
        return (a <= 0 or b <= 0.0)
            ? 0.0
            : ((p.x * p.x) / (a * a) + (p.y * p.y) / (b * b) <= 1.0 ? 1.0 : 0.0);
    };
}

// Generuje powierzchniĂÂ z prostokÄtem o Ĺrodku w punkcie (0, 0) o brzegu
// dĹugoĹci 2a wzdĹuĹź X (domyĹlnie a = 1) i o brzegu dĹugoĹci 2b wzdĹuĹź osi Y
// (domyĹlnie b = 1), czyli f(x, y) = 1, gdy (x, y) leĹźy wewnÄtrz lub na brzegu
// prostokÄta, zaĹ f(x, y) = 0, w. p.p. JeĹli a <= 0 lub b <= 0, to f(x, y) = 0.
inline Surface rectangle(Real a = 1, Real b = 1) {
    return [a, b](Point p) -> Real {
        return (a <= 0 or b <= 0)
               ? 0.0
               : ((std::abs(p.x) <= a and std::abs(p.y) <= b) ? 1.0 : 0.0);
    };
}

// Generuje powierzchniÄ z paskami o szerokoĹci s, uĹoĹźonymi wzdĹuĹź osi X,
// rĂłwnolegĹymi do osi Y. Zachodzi f(x, y) = 1, gdy 0 < x <= s oraz f(x, y) = 0,
// gdy -s < x <= 0 itd. JeĹli s <= 0, to f(x, y) = 0.
inline Surface stripes(Real s) {
    return [s](Point p)-> Real {
        return s <= 0 ? 0 : std::abs(std::fmod(ceil(p.x / s), 2.0));
    };
}

// Obraca dziedzinÄ powierzchni o kÄt wyraĹźony w stopniach.
inline Surface rotate(const Surface &f, Real deg) {
    return [f, deg](Point p) -> Real {
        // convert to radians
        const Real rad = (-deg) * (std::numbers::pi / 180);

        const Real s = sin(rad);
        const Real c = cos(rad);

        const Real new_x = p.x * c - p.y * s;
        const Real new_y = p.x * s + p.y * c;

        return f(Point(new_x, new_y));
    };
}

// Przesuwa dziedzinÄ powierzchni o wektor v (typu Point).
inline Surface translate(const Surface &f, Point v) {
    return [f, v](Point p) -> Real {
        return f(Point(p.x - v.x, p.y - v.y));
    };
}

// Skaluje dziedzinÄ powierzchni parÄ wspĂłĹczynnikĂłĹw s (typu Point).
inline Surface scale(const Surface &f, Point s) {
    return [f, s](Point p) -> Real {
        return f(Point(p.x / s.x, p.y / s.y));
    };
}

// Odwraca dziedzinÄ powierzchni, tzn. zamienia (x, y) na (y, x).
inline Surface invert(const Surface &f) {
    return [f](Point p) -> Real {
        return f(Point(p.y, p.x));
    };
}

// Odbija dziedzinÄ powierzchni wzglÄdem osi Y, tzn. zamienia x na -x.
inline Surface flip(const Surface &f) {
    return [f](Point p) -> Real {
        return f(Point(-p.x, p.y));
    };
}

// MnoĹźy wartoĹci podanej powierzchni f przez podanÄ wartoĹÄ c.
inline Surface mul(const Surface &f, Real c) {
    return [f, c](Point p) {
        return f(p) * c;
    };
}

// Dodaje podanÄ wartoĹÄ c do wartoĹci podanej powierzchni f.
inline Surface add(const Surface &f, Real c) {
    return [f, c](Point p) {
        return f(p) + c;
    };
}

// oblicza wartoĹÄ funkcji h(f1(p), ..., fs(p)).
// wywoĹanie evaluate bez parametrĂłw nie powinno siÄ kompilowaÄ.
template<typename H>
inline decltype(auto) evaluate(const H &h) {
    return [h](Point) {
        return h();
    };
}

template<typename H, typename... Fs>
inline decltype(auto) evaluate(const H &h, const Fs&... fs) {
    return [h, fs...](Point p) {
        return h(fs(p)...);
    };
};

// generuje zĹoĹźenie funkcji fs(...(f2(f1))...).
inline auto compose() {
    return [](auto x) {
        return x;
    };
}

template <typename F>
inline auto compose(const F &f) {
    return [f](auto x) {
        return f(x);
    };
}

template <class F, class... Fs>
inline auto compose(const F &f, const Fs&... fs) {
    return [f, fs...](auto x) {
        return compose(fs...)(f(x));
    };
}

#endif //SURFACES_H
