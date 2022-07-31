#pragma once

#include <filesystem>
#include <vector>
#include <unordered_set>

typedef unsigned node;

class Graph {
 public:
  Graph(const std::filesystem::path& input_file);

  inline unsigned n() const { return m_n; }

  const std::vector<node>& nodes() const;

  const std::vector<node>& neighbors(node v) const;

  unsigned degree(node v) const;

  void permute_nodes(std::vector<node> old_to_new_id);

  std::vector<std::vector<bool>> adj_matrix() const;
  std::vector<std::unordered_set<node>> adj_set() const;

 private:
  unsigned m_n;
  std::vector<std::vector<node>> m_adj_list;
  std::vector<node> m_nodes;
};
