#ifndef LOCALISATION_HPP
#define LOCALISATION_HPP

#include <vector>
#include <array>
#include <optional>
#include <functional>
#include "types.hpp"
#include "rng.hpp"

class Localisation {
private:
    unsigned int N;                         // number of particles
    Binmap map;                             // map - m in pseudocode
    std::vector<double> angles;             // laser angles
    std::size_t width;                      // width of the map
    std::size_t height;                     // height of the map

    unsigned int number_of_threads;

    RNG rng;                                // random number generator

    double sensor_noise, forward_noise, turn_noise;

    // parameters for augmented MCL
    static double alpha_slow, alpha_fast;
    double w_slow, w_fast;

    // parameters for adaptive MCL
    static unsigned int M_min, M_max;
    static double epsilon, delta;
public:
    Localisation(const Binmap & map, const std::vector<double> & angles);

    // Localisation algorithms
    Pose mcl(std::vector<Particle> & particles, const std::array<double, 3> & u_t, const std::vector<double> & z_t);
    Pose augmented_mcl(std::vector<Particle> & particles, const std::array<double, 3> & u_t, const std::vector<double> & z_t);
    Pose mmcl(std::vector<Particle> & particles, const std::array<double, 3> &u_t, const std::vector<double> &z_t);

    void generate_particles(std::vector<Particle> & particles, Problem mode, Pose initial_position = {});

    double update_particles(std::vector<Particle> & particles, const std::array<double, 3> & u_t, const std::vector<double> & z_t, unsigned int from, unsigned int to);

    void motion_update(Particle & particle, const std::array<double, 3> & u_t);

    std::vector<double> measure_particle(Particle & particle) const;

    [[nodiscard]] double calculate_distance(double x, double y, double h) const;

    double measurement_model(Particle & particle, const std::vector<double> & z_t) const;

    void normalize_weights(std::vector<Particle> & particles, double total_norm);

    // resampling algorithms
    std::vector<Particle> systematic_resampling(std::vector<Particle> & particles);
    std::vector<Particle> stratified_resampling(std::vector<Particle> & particles, const std::vector<double> & randoms) const;
    std::vector<Particle> modified_stratified_resampling(std::vector<Particle> & particles, const std::vector<double> & randoms, double prob);
    std::vector<Particle> kld_resampling(const std::vector<Particle> & particles, double probability);

    Pose estimate_position(const std::vector<Particle> & particles);

    // setters
    void set_threads(unsigned int thread_count);
    void set_map(const Binmap & enviroment);
    void set_angles(const std::vector<double> & angles);
    static void set_paramaters(double a_slow, double a_fast, double eps, double d, unsigned int min, unsigned int max);
private:
    Particle generate_random_particle();

    std::optional<std::tuple<int, int, int>> get_bin(const std::vector<std::vector<std::vector<bool>>> & bins, const Particle & particle) const;

    [[nodiscard]] bool legal_map_position(double x, double y) const;

    Pose static_state_position_estimation(std::vector<Particle> & particles);

    std::function<double(unsigned int, unsigned int)> update_particle_section(std::vector<Particle>& particles, const std::array<double, 3> & u_t, const std::vector<double> & z_t);
};

#endif // LOCALISATION_HPP

