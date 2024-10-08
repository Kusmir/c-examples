// ObsĹuga generowania rysunkĂłw powierzchni w uproszczonym jÄzyku PostScript

#ifndef PS_PLOT_H
#define PS_PLOT_H

#include <cstddef>
#include <ostream>
#include "surfaces.h"

constexpr Real DEFAULT_MIN = Real(-1.0);
constexpr Real DEFAULT_MAX = Real(1.0);
constexpr size_t DEFAULT_COUNT = 40;
constexpr size_t DEFAULT_SCALE = 175;

void create_PS_plot(std::ostream & out, Surface const f,
    Real const x_min = DEFAULT_MIN,       // poczÄtek rysowanego obszaru na osi X
    Real const x_max = DEFAULT_MAX,       // koniec rysowanego obszaru na osi X
    Real const y_min = DEFAULT_MIN,       // poczÄtek rysowanego obszaru na osi Y
    Real const y_max = DEFAULT_MAX,       // koniec rysowanego obszaru na osi Y
    size_t const x_count = DEFAULT_COUNT, // liczba rysowanych punktĂłw na osi X (> 0)
    size_t const y_count = DEFAULT_COUNT, // liczba rysowanych punktĂłw na osi Y (> 0)
    size_t const scale = DEFAULT_SCALE    // skala oznaczjÄca liczbÄ punktĂłw ekranowych na jednostkÄ typu Real
);

#endif
