#include <iostream>

#include "app.hpp"
#include "graph.hpp"
#include "random.hpp"
#include "timer.hpp"
#include "triangles.hpp"

int main(int argc, char** argv) {
  App app{"Count the triangles of a graph."};

  std::filesystem::path input_file;
  app.arg(input_file, "input_file", "Name of the file containing the graph");

  std::string algo = "clever";
  app.arg(algo, "--algo", "The algorithm that should be used.",
          {"clever", "clever_skip", "clever_sort_skip", "brute_force"});

  app.arg_seed();
  
  app.arg_header("graph,triangles,algo,time");

  app.parse(argc, argv);

  Graph G(input_file);

  // shuffle the nodes of the graph
  std::vector<node> permut(G.n());
  std::iota(permut.begin(), permut.end(), 0);
  Random::shuffle(permut);
  G.permute_nodes(permut);

  unsigned res = 0;
  Timer::create_timer("triangles");
  Timer::start_timer("triangles");
  if (algo == "clever") {
    res = triangles_clever(G);
  } else if (algo == "clever_skip") {
    res = triangles_clever_skip(G);
  } else if (algo == "clever_sort_skip") {
    res = triangles_clever_sort_skip(G);
  } else if (algo == "brute_force") {
    res = triangles_brute_force(G);
  }
  Timer::stop_timer("triangles");

  std::cout << input_file.filename() << "," << res << "," << algo << ","
            << Timer::time("triangles").count() << std::endl;

  return 0;
}
