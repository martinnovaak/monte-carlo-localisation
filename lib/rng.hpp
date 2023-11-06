#ifndef RNG_HPP
#define RNG_HPP

#include <random>

class RNG
{
public:
    RNG() {
        engine = std::mt19937(std::random_device{}());
        standard_normal_dist = std::normal_distribution<double>(0.0, 1.0);
        random_uniform = std::uniform_real_distribution<double>(0.0, 1.0);
    };

    double generate_uniform_number() {
        return random_uniform(engine);
    };

    double generate_normal_number() {
        return standard_normal_dist(engine);
    };

    double generate_triangle_number() {
        double u1 = generate_uniform_number();
        double u2 = generate_uniform_number();

        return u1 + u2 - 1.0;
    }

private:
    std::mt19937 engine;
    std::uniform_real_distribution<double> random_uniform;
    std::normal_distribution<double> standard_normal_dist;
};

#endif // RNG_HPP
