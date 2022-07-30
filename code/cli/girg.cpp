

#include <limits>
#include <utility>
#include <vector>
#include <omp.h>

#include "CLI11.hpp"
#include "app.hpp"
#include "girgs/Generator.h"

std::vector<double> power_law_weights(unsigned n, double ple) {
  std::vector<double> weights(n);
  double exponent = - 1 / (ple - 1);
  for (unsigned i = 0; i < n; ++i) {
    weights[i] = std::pow(i + 1, exponent);
  }
  return weights;
}

int main(int argc, char** argv) {
  App app{"Generating geometric inhomogeneous random graphs."};

  std::string output_file = "output_graph";
  app.arg(output_file, "output_file", "Name of the resulting graph file.");

  unsigned n = 100;
  app.arg(n, "--nr_nodes,-n",
          "Number of nodes of the generated graph (default: 100).");

  double deg = 15;
  app.arg(deg, "--deg", "Expected average degree (default: 15).");

  unsigned dim = 1;
  app.arg(dim, "--dim", "The Dimension of the ground space (default: 1).",
          CLI::PositiveNumber);

  double ple = 2.5;
  app.arg(ple, "--ple", "The power-law exponent (default: 2.5).",
          CLI::PositiveNumber & !CLI::Range(0.0, 2.0));

  double T = 0;
  app.arg(T, "--temperature,-T", "The temperature (default: 0).",
          CLI::Range(0.0, 1.0));
  
  bool square = false;
  app.arg(square, "--square", "Use a square instead of a torus as ground space.");

  app.arg_header("n,deg,dim,ple,T,alpha,square,seed");

  app.arg_seed();

  app.parse(argc, argv);

  // alpha = inverse temperature
  double alpha = T > 0 ? 1 / T : std::numeric_limits<double>::infinity();

  // three seeds for the three steps
  // unsigned wseed = Random::natural_number();
  unsigned pseed = Random::natural_number();
  unsigned sseed = Random::natural_number();

  omp_set_num_threads(1);
  
  // generate stuff
  // auto weights = girgs::generateWeights(n, ple, wseed, false);
  auto weights = power_law_weights(n, ple);
  auto positions = girgs::generatePositions(n, dim, pseed);
  girgs::scaleWeights(weights, deg, dim, alpha);
  if (square) {
    // scale all positions with 0.5 to prevent wrapping of the torus
    for (auto& pos : positions) {
      for (auto& coordinate : pos) {
        coordinate *= 0.5;
      }
    }
    // scale all weights to accommodate for the lower distances
    double factor = std::pow(0.5, dim);
    for (auto& weight : weights) {
      weight *= factor;
    }
  }
  auto edges = girgs::generateEdges(weights, positions, alpha, sseed);

  // write graph to file
  std::ofstream out{output_file};
  for (auto& e : edges) {
    out << e.first << ' ' << e.second << '\n';
  }

  // output parameters
  std::cout << n << "," << deg << "," << dim << "," << ple << "," << T << ","
            << alpha << "," << square << "," << Random::get_seed() << std::endl;

  return 0;
}
