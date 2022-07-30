#pragma once

#include <filesystem>
#include <vector>

typedef unsigned node;

class Graph {
 public:
  Graph(const std::filesystem::path& input_file);
 
  unsigned n() const;

  const std::vector<node>& nodes() const;
  
  const std::vector<node>& neighbors(node v) const;

  unsigned degree(node v) const;

  void permute_nodes(std::vector<node> old_to_new_id);
  
 private:
  std::vector<std::vector<node>> m_adj_list;
  std::vector<node> m_nodes;
};
