#include "random.hpp"

#include <chrono>
#include <random>

Random::Random()
    : m_seed(std::chrono::system_clock::now().time_since_epoch().count()),
      m_engine(m_seed) {}

void Random::set_seed(unsigned seed) {
  Random& r = get_singleton();
  r.m_seed = seed;
  r.m_engine = std::default_random_engine(r.m_seed);
}

unsigned Random::get_seed() {
  Random& r = get_singleton();
  return r.m_seed;
}

unsigned Random::natural_number(unsigned max) {
  Random& r = get_singleton();
  std::uniform_int_distribution<unsigned> distr(0, max);
  return distr(r.m_engine);
}

bool Random::coin_flip(double p) {
  Random& r = get_singleton();
  std::uniform_real_distribution<double> distr;
  return distr(r.m_engine) < p;
}

unsigned Random::geometric_skip(double p) {
  Random& r = get_singleton();
  std::geometric_distribution<int> distr(p);
  return distr(r.m_engine);
}
