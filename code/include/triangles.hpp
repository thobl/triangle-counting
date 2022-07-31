#pragma once

#include <vector>
#include "graph.hpp"

unsigned triangles_clever(const Graph& G);

unsigned triangles_clever_skip(const Graph& G);

unsigned triangles_clever_sort_skip(Graph& G);

unsigned triangles_brute_force_matrix(const Graph& G);

unsigned triangles_brute_force_matrix_skip(const Graph& G);

unsigned triangles_brute_force_hash_skip(const Graph& G);

unsigned triangles_brute_force_clever_skip(const Graph& G);
