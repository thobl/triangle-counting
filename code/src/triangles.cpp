#include "triangles.hpp"

#include "timer.hpp"

#include <algorithm>
#include <vector>

unsigned triangles_clever(const Graph& G) {
  unsigned count = 0;
  std::vector<bool> marked(G.n(), false);

  for (node u : G.nodes()) {
    // mark neighbors of u
    for (node v : G.neighbors(u)) {
      marked[v] = true;
    }

    for (node v : G.neighbors(u)) {
      for (node w : G.neighbors(v)) {
        if (marked[w]) {
          count++;
        }
      }
    }

    // unmark neighbors of u
    for (node v : G.neighbors(u)) {
      marked[v] = false;
    }
  }
  return count / 6;
}

unsigned triangles_clever_skip(const Graph& G) {
  // only count triangles with u < v < w
  unsigned count = 0;
  std::vector<bool> marked(G.n(), false);

  for (node u : G.nodes()) {
    // mark neighbors of u
    for (node v : G.neighbors(u)) {
      marked[v] = true;
    }

    for (node v : G.neighbors(u)) {
      if (u >= v) continue;
      for (node w : G.neighbors(v)) {
        if (marked[w] && v < w) {
          count++;
        }
      }
    }

    // unmark neighbors of u
    for (node v : G.neighbors(u)) {
      marked[v] = false;
    }
  }
  return count;
}

unsigned triangles_clever_sort_skip(Graph& G) {
  // sort the nodes by degree
  std::vector<node> nodes = G.nodes();
  std::sort(nodes.begin(), nodes.end(),
            [&](node u, node v) { return G.degree(u) > G.degree(v); });

  // map from the old id (value in `nodes`) to the new id (position in
  // `nodes`)
  std::vector<node> old_to_new_id(G.n());
  for (unsigned pos = 0; pos < G.n(); pos++) {
    old_to_new_id[nodes[pos]] = pos;
  }
  G.permute_nodes(old_to_new_id);
  return triangles_clever_skip(G);
}

unsigned triangles_brute_force_matrix(const Graph& G) {
  Timer::stop_timer("triangles");
  auto mtx = G.adj_matrix();
  Timer::start_timer("triangles");
  unsigned count = 0;
  for (node u = 0; u < G.n(); ++u) {
    for (node v = u + 1; v < G.n(); ++v) {
      for (node w = v + 1; w < G.n(); ++w) {
        if (mtx[u][w] && mtx[v][w] && mtx[u][v]) {
          // note: if the `mtx[u][v]` comes first, the compiler seems
          // to notice that the inner loop can be skipped if
          // `mtx[u][v]` is evaluated before the loop (basically like
          // in `triangles_brute_force_matrix_skip`)
          count++;
        }
      }
    }
  }
  return count;
}

unsigned triangles_brute_force_matrix_skip(const Graph& G) {
  Timer::stop_timer("triangles");
  auto mtx = G.adj_matrix();
  Timer::start_timer("triangles");  
  unsigned count = 0;
  for (node u = 0; u < G.n(); ++u) {
    for (node v = u + 1; v < G.n(); ++v) {
      if (!mtx[u][v]) continue;
      for (node w = v + 1; w < G.n(); ++w) {
        if (mtx[u][w] && mtx[v][w]) {
          count++;
        }
      }
    }
  }
  return count;
}

unsigned triangles_brute_force_hash_skip(const Graph& G) {
  Timer::stop_timer("triangles");
  auto sets = G.adj_set();
  Timer::start_timer("triangles");
  unsigned count = 0;
  for (node u = 0; u < G.n(); ++u) {
    for (node v = u + 1; v < G.n(); ++v) {
      if (!sets[u].contains(v)) continue;
      for (node w = v + 1; w < G.n(); ++w) {
        if (sets[u].contains(w) && sets[v].contains(w)) {
          count++;
        }
      }
    }
  }
  return count;
}

unsigned triangles_brute_force_clever_skip(const Graph& G) {
  std::vector<bool> u_marked(G.n(), false);
  std::vector<bool> v_marked(G.n(), false);
  unsigned count = 0;
  for (node u = 0; u < G.n(); ++u) {
    // mark neighbors of u
    for (node x : G.neighbors(u)) {
      u_marked[x] = true;
    }

    for (node v = u + 1; v < G.n(); ++v) {
      if (!u_marked[v]) continue;

      // mark neighbors of v
      for (node x : G.neighbors(v)) {
        v_marked[x] = true;
      }

      for (node w = v + 1; w < G.n(); ++w) {
        if (u_marked[v] && u_marked[w] && v_marked[w]) {
          count++;
        }
      }

      // unmark neighbors of v
      for (node x : G.neighbors(v)) {
        v_marked[x] = false;
      }
    }

    // unmark neighbors of u
    for (node x : G.neighbors(u)) {
      u_marked[x] = false;
    }
  }
  return count;
}
