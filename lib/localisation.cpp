#include "localisation.hpp"
#include "formulas.hpp"
#include <iostream>
#include <algorithm>
#include <future>

// initialization of static values
double Localisation::alpha_slow = 0.05;
double Localisation::alpha_fast = 0.50;
unsigned int Localisation::M_min = 100;
unsigned int Localisation::M_max = 100'000;
double Localisation::epsilon = 0.01;
double Localisation::delta = 0.01;


Localisation::Localisation(const Binmap & map, const std::vector<double> & angles)
        : map(map), angles(angles), number_of_threads(8), w_slow(0), w_fast(0), N(1)
{
    rng = RNG();

    height = map.size();
    if(!map.empty()) {
        width = map[0].size();
    }
}

// classic MCL algorithm - unable to solve kidnapped robot problem
Pose Localisation::mcl(std::vector<Particle> & particles, const std::array<double, 3> & u_t, const std::vector<double> & z_t)
{
    // Variance reduction
    // When the state is static x_t == x_(t-1)
    if (u_t[0] == 0.0 && u_t[1] == 0.0 && u_t[2] == 0.0) {
        return estimate_position(particles); // Return the estimated position without any updates or resampling
    }

    this->N = particles.size();
    unsigned int particles_per_thread = N / number_of_threads;
    std::vector<std::future<double>> futures;
    double total_norm = 0.0;

    // Define a lambda function to update particles and weights within a specified range
    auto update_func = [&](unsigned int start, unsigned int end) {
        return update_particles_and_weights(particles, u_t, z_t, start, end);
    };

    // Parallelize the particle update process by dividing the work among multiple threads
    for (unsigned int i = 0; i < number_of_threads; i++) {
        unsigned int start = i * particles_per_thread;
        unsigned int end = (i == number_of_threads - 1) ? N : (i + 1) * particles_per_thread;
        futures.push_back(std::async(std::launch::async, update_func, start, end));
    }

    // Join threads and calculate the total weight normalization factor
    for (auto & future : futures) {
        total_norm += future.get();
    }

    // Normalize particle weights
    for (auto & particle : particles) {
        particle.weight /= total_norm;
    }

    Pose estimate = estimate_position(particles);

    particles = systematic_resampling(particles);

    return estimate;
}

Pose Localisation::augmented_mcl(std::vector<Particle> &particles, const std::array<double, 3> &u_t, const std::vector<double> &z_t)
{
    // Variance reduction
    // When the state is static x_t == x_(t-1)
    if(u_t[0] == 0.0 && u_t[1] == 0.0 && u_t[2] == 0.0) {
        return estimate_position(particles); // Return the estimated position without any updates or resampling
    }

    this->N = particles.size();
    unsigned int particles_per_thread = N / number_of_threads;
    std::vector<std::future<double>> futures;
    double total_norm = 0.0;

    auto update_func = [&](unsigned int start, unsigned int end) {
        return update_particles_and_weights(particles, u_t, z_t, start, end);
    };

    // Parallelize the particle update process by dividing the work among multiple threads
    for (unsigned int i = 0; i < number_of_threads; i++) {
        unsigned int start = i * particles_per_thread;
        unsigned int end = (i == number_of_threads - 1) ? N : (i + 1) * particles_per_thread;
        futures.push_back(std::async(std::launch::async, update_func, start, end));
    }

    for (auto & future : futures) {
        total_norm += future.get();
    }

    for(auto & particle : particles) {
        particle.weight /= total_norm;
    }

    std::vector<double> randoms(N);
    for(auto & random : randoms) {
        random = rng.generate_uniform_number();
    }

    // average weight
    double w_avg = total_norm / N;

    w_slow += alpha_slow * (w_avg - w_slow);
    w_fast += alpha_fast * (w_avg - w_fast);

    double w_diff = 1.0 - w_fast / w_slow;

    Pose estimate = estimate_position(particles);

    particles = modified_stratified_resampling(particles, randoms, w_diff);

    return estimate;
}

// Adaptive monte carlo localisation
Pose Localisation::mmcl(std::vector<Particle> & particles, const std::array<double, 3> & u_t, const std::vector<double> & z_t)
{
    // x_t == x_{t-1}
    if(u_t[0] == 0.0 && u_t[1] == 0.0 && u_t[2] == 0.0) {
        return estimate_position(particles);
    }

    this->N = particles.size();
    unsigned int particles_per_thread = N / number_of_threads;
    std::vector<std::future<double>> futures;
    double total_norm = 0.0;

    auto update_func = [&](unsigned int start, unsigned int end) {
        return update_particles_and_weights(particles, u_t, z_t, start, end);
    };

    // Parallelize the particle update process by dividing the work among multiple threads
    for (unsigned int i = 0; i < number_of_threads; i++) {
        unsigned int start = i * particles_per_thread;
        unsigned int end = (i == number_of_threads - 1) ? N : (i + 1) * particles_per_thread;
        futures.push_back(std::async(std::launch::async, update_func, start, end));
    }

    for (auto & future : futures) {
        total_norm += future.get();
    }

    for(auto & particle : particles) {
        particle.weight /= total_norm;
    }

    // average weight
    double w_avg = total_norm / N;

    w_slow += alpha_slow * (w_avg - w_slow);
    w_fast += alpha_fast * (w_avg - w_fast);

    double probability = std::max(0.0, 1.0 - w_fast / w_slow);

    Pose estimate = estimate_position(particles);

    particles = kld_resampling(particles, probability);

    return estimate;
}

void Localisation::generate_particles(std::vector<Particle> & particles, Problem mode, Pose initial_position)
{
    this->N = particles.size();
    this->height = map.size();

    if (!map.empty()) {
        this->width = map[0].size();
    }

    double x, y, h;
    if (mode == Problem::LOCAL) {
        auto [init_x, init_y, init_h] = initial_position;
        for(unsigned int i = 0; i < N; i++) {
            h = init_h + rng.generate_normal_number() * 0.1;
            do {
                x = fmod((init_x + cos(h) * rng.generate_normal_number() * 10), static_cast<double>(width));
                y = fmod((init_y + sin(h) * rng.generate_normal_number() * 10), static_cast<double>(height));
            } while(!legal_map_position(x, y));
            particles[i] = Particle(x, y, h);
        }
    } else {
        for(unsigned int i = 0; i < N; i++) {
            particles[i] = generate_random_particle();
        }
    }
}

double Localisation::update_particles_and_weights(std::vector<Particle> & particles, const std::array<double, 3> &u_t, const std::vector<double> &z_t, unsigned int from, unsigned int to)
{
    double total_weight = 0.0;
    for (unsigned int i = from; i < to; i++) {
        Particle & particle = particles[i];
        motion_update(particle, u_t);
        total_weight += measurement_model(particle, z_t);
    }
    return total_weight;
}

void Localisation::motion_update(Particle & particle, const std::array<double, 3> & u_t)
{
    // Normal distribution
    particle.x += u_t[0] + cos(particle.h) * rng.generate_normal_number() * 20;
    particle.y += u_t[1] + sin(particle.h) * rng.generate_normal_number() * 20;
    particle.h += u_t[2] + rng.generate_normal_number() * 0.1;

    particle.h = fmod(particle.h, 360.0);
}

std::vector<double> Localisation::measure_particle(Particle & particle) const {
    auto [x, y, h] = particle.getPose();
    std::vector<double> distances(angles.size());

    for (unsigned int j = 0; j < angles.size(); j++) {
        distances[j] = calculate_distance(x, y, h+angles[j]);
    }

    return distances;
}

double Localisation::calculate_distance(double x, double y, double h) const {
    double distance = 0.0;
    double ax = cos(h), ay = sin(h);
    double dx = 0.0, dy = 0.0;
    int mx, my;

    if (!legal_map_position(x, y)) {
        return -1.0;
    }

    do {
        distance += 1.0;
        dx += ax;
        dy += ay;

        mx = static_cast<int>(x + dx);
        my = static_cast<int>(y + dy);
    } while(!map[my][mx]);

    return distance;
}

double Localisation::measurement_model(Particle &particle, const std::vector<double> &z_t) const
{
    std::vector<double> distances = measure_particle(particle);

    double p = 1.0, sigma_hit = 0.5;
    for (unsigned int i = 0; i < z_t.size(); i++) {
        double a = (z_t[i] - distances[i]) / 100.0;
        p *= std::exp(-0.5 * a * a) / sigma_hit;
    }
    p += 1.0 / N;
    particle.weight = p;
    return p;
}

// systematic resampling
std::vector<Particle> Localisation::systematic_resampling(std::vector<Particle> & particles)
{
    std::vector<Particle> new_particles(N);
    double M = 1.0 / N;
    double U;
    double r = rng.generate_uniform_number();
    double c = particles[0].weight;

    for(unsigned int i = 0, j = 0; j < N; j++) {
        U = (r + j) * M;
        while(U > c) {
            c += particles[++i].weight;
        }
        new_particles[j] = particles[i];
    }

    return new_particles;
}

// stratified resampling with pregenerated N random values
std::vector<Particle> Localisation::stratified_resampling(std::vector<Particle> & particles, const std::vector<double> & randoms) const
{
    std::vector<Particle> new_particles(N);
    double M = 1.0 / N;
    double U;
    double c = particles[0].weight;

    for (unsigned int i = 0, j = 0; j < N; j++) {
        U = (randoms[j] + j) * M;
        while(U > c) {
            i++;
            c += particles[i].weight;
        }
        new_particles[j] = particles[i];
    }

    return new_particles;
}

// modified stratified resampling with injection of random samples
// prob = probability for injection of random sample
std::vector<Particle> Localisation::modified_stratified_resampling(std::vector<Particle> & particles, const std::vector<double> & randoms, double prob)
{
    if (prob <= 0.0) { // if prob is 0 -> do just stratified resampling
        return stratified_resampling(particles, randoms);
    }

    std::vector<Particle> new_particles(N);

    double M = 1.0 / N;
    double U, u;
    double c = particles[0].weight;

    for(unsigned int i = 0, j = 0; j < N; j++) {
        u = rng.generate_uniform_number();
        if(u < prob) {
            new_particles[j] = generate_random_particle();
        } else {
            U = (u + j) * M;
            while(U > c) {
                i++;
                c += particles[i].weight;
            }
            new_particles[j] = particles[i];
        }
    }
    return new_particles;
}

std::vector<Particle> Localisation::kld_resampling(const std::vector<Particle> & particles, double probability)
{
    std::vector<std::vector<std::vector<bool>>> bins(24, std::vector<std::vector<bool>>(36, std::vector<bool>(24, false)));
    std::vector<Particle> new_particles;

    unsigned int M_X = M_min;
    unsigned int M_prev;
    unsigned int k = 0; // number of occupied bins
    unsigned int n = 0; // number of resampled particles

    do {
        double M = 1.0 / M_X;
        M_prev = M_X;
        int i = 0;
        double cumulative_weight = particles[0].weight; // cumulative weight

        while (n < M_prev) {
            double random = rng.generate_uniform_number();
            Particle new_particle;

            if (random < std::max(0.0, probability)) {
                new_particle = generate_random_particle();
            } else {
                double U = (random + n) * M;
                while (U > cumulative_weight) {
                    i++;
                    cumulative_weight += particles[i].weight;
                }
                new_particle = particles[i];
                //new_particle.weight = 1.0;
            }

            new_particles.push_back(new_particle);
            std::optional<std::tuple<int, int, int>> bin = get_bin(bins, new_particle);

            if (!bin) {
                k += 1;
                M_X = std::clamp(static_cast<unsigned int>(chi_squared(k - 1, 1.0 - delta) / (2 * epsilon)), M_min, M_max);
            } else {
                auto [x, y, h] = *bin;
                if (!bins[y][x][h]) {
                    k += 1;
                    bins[y][x][h] = true;
                    M_X = std::clamp(static_cast<unsigned int>(chi_squared(k - 1, 1.0 - delta) / (2 * epsilon)), M_min, M_max);
                }
            }

            n++;
        }
    } while(M_X != M_prev);

    return new_particles;
}

Pose Localisation::estimate_position(const std::vector<Particle> & particles)
{
    double x = 0.0, y = 0.0, h = 0.0, weight;
    for (const auto & particle : particles) {
        weight = particle.weight;
        x += particle.x * weight;
        y += particle.y * weight;
        h += particle.h * weight;
    }
    return {x, y, h};
}

Particle Localisation::generate_random_particle()
{
    double x, y, h;
    h = rng.generate_uniform_number() * 360.0;
    do {
        x = rng.generate_uniform_number() * width;
        y = rng.generate_uniform_number() * height;
    } while (!legal_map_position(x, y));
    return {x, y, h};
}

void Localisation::set_threads(unsigned int thread_count)
{
    number_of_threads = thread_count;
}

void Localisation::set_map(const Binmap & enviroment)
{
    height = enviroment.size();
    if (!enviroment.empty()) {
        width = enviroment[0].size();
    }
    map = enviroment;
}

void Localisation::set_angles(const std::vector<double> & angles)
{
    this->angles = angles;
}

void Localisation::set_paramaters(double a_slow, double a_fast, double eps, double d, unsigned int min, unsigned int max)
{
    alpha_slow = a_slow;
    alpha_fast = a_fast;
    epsilon = eps;
    delta = d;
    M_min = min;
    M_max = max;
}

std::optional<std::tuple<int, int, int>> Localisation::get_bin(const std::vector<std::vector<std::vector<bool>>> & bins, const Particle & particle) const
{
    int x, y, h;
    std::tie(x, y, h) = particle.getPose();

    if (x >= static_cast<int>(width) || y >= static_cast<int>(height) || x < 0 || y < 0) {
        return std::nullopt;
    }

    int y_bins = std::max(static_cast<int>(bins.size()) - 1, 0);
    int x_bins = std::max(static_cast<int>(bins[0].size()) - 1, 0);
    int h_bins = std::max(static_cast<int>(bins[0][0].size()) - 1, 0);

    y = std::clamp(y * y_bins / static_cast<int>(height), 0, y_bins - 1);
    x = std::clamp(x * x_bins / static_cast<int>(width),  0, x_bins - 1);
    h = std::clamp(h * h_bins / 360, 0, h_bins - 1);

    return std::make_tuple(x, y, h);
}

bool Localisation::legal_map_position(double x, double y) const
{
    return x >= 0.0 && y >= 0.0 && x < static_cast<double>(width) && y < static_cast<double>(height) && !map[static_cast<int>(y)][static_cast<int>(x)];
}
