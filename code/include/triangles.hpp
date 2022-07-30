#pragma once

#include "graph.hpp"

unsigned triangles_clever(const Graph& G);

unsigned triangles_clever_skip(const Graph& G);

unsigned triangles_clever_sort_skip(Graph& G);

unsigned triangles_brute_force(const Graph& G);
