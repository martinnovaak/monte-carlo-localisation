#ifndef FORMULAS_HPP
#define FORMULAS_HPP

#include <cmath>

//https://www.johndcook.com/blog/normal_cdf_inverse/
double normal_cdf_inverse(double p)
{
    double t = std::sqrt(-2.0 * std::log(std::min(p, 1 - p)));
    t -= (((0.010328 * t + 0.802853) * t + 2.515517) /
          (((0.001308 * t + 0.189269) * t + 1.432788) * t + 1.0));
    return p >= 0.5 ? t : -t;
}

// Wilson Hilferty transormation
// Fox, Dieter. (2001). KLD-Sampling: Adaptive Particle Filters 713-720.
double chi_squared(double x, double y)
{
    double a = 2 / x;
    double b = 1 - a + std::sqrt(a) * normal_cdf_inverse(y);
    return x * std::pow(b, 3);
}

#endif // FORMULAS_HPP
