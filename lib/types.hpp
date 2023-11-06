#ifndef UTILS_HPP
#define UTILS_HPP

#include <tuple>
#include <vector>

using Pose   = std::tuple<double, double, double>;
using Binmap = std::vector<std::vector<bool>>;

enum Problem : int {
    LOCAL = 0, GLOBAL = 1, KIDNAPPED_ROBOT = 2, MMCL = 3
};

struct Particle {
    double x, y, h; // coordinates
    double weight;

    Particle() : x{}, y{}, h{}, weight(1.0) {}
    Particle(double x, double y, double h, double w) : x(x), y(y), h(h), weight(w) {}
    [[nodiscard]] Pose getPose() const { return { x, y, h }; }
};

#endif // UTILS_HPP
