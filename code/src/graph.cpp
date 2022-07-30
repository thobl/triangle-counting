#include "graph.hpp"
#include <fstream>
#include <numeric>
#include <vector>


Graph::Graph(const std::filesystem::path& input_file) {
  std::ifstream in{input_file};
  
  // read pairs of nodes until file ends
  node u, v;
  while (in >> u && in >> v) {
    // increase adj list array if necessary
    unsigned max_id = std::max(u, v);
    if (max_id >= m_adj_list.size()) {
      m_adj_list.resize(max_id + 1);
    }

    // actually create the edge
    m_adj_list[u].push_back(v);
    m_adj_list[v].push_back(u);
  }

  // create the node set
  m_nodes = std::vector<node>(m_adj_list.size());
  std::iota(m_nodes.begin(), m_nodes.end(), 0);
}

unsigned Graph::n() const {
  return m_nodes.size();
}

const std::vector<node>& Graph::nodes() const {
  return m_nodes;
}

const std::vector<node>& Graph::neighbors(node v) const {
  return m_adj_list[v];
}

unsigned Graph::degree(node v) const {
  return m_adj_list[v].size();
}

void Graph::permute_nodes(std::vector<node> old_to_new_id) {
  std::vector<std::vector<node>> new_adj_list{n()};
  for (node u : nodes()) {
    for (node v : neighbors(u)) {
      new_adj_list[old_to_new_id[u]].push_back(old_to_new_id[v]);
    }
  }
  m_adj_list = new_adj_list;
}
