//
// Created by a on 8/16/20.
//

#ifndef DIJKSTRA_DIJKSTRA_H
#define DIJKSTRA_DIJKSTRA_H

#include <memory>
#include <queue>
#include <functional>
#include <limits>

#include "Graph/Input/Structure.h"

class Dijkstra {

private:
    const Structure &m_graph;

    struct Graph_node_less {
        auto operator()(std::pair<std::int64_t, std::shared_ptr<Vertex>> const a,
                        std::pair<std::int64_t, std::shared_ptr<Vertex>> const b) const
        -> bool { return a.first < b.first; }
    };

    std::priority_queue<std::pair<std::int64_t, std::shared_ptr<Vertex>>, std::vector<std::pair<std::int64_t, std::shared_ptr<Vertex>>>, Graph_node_less> m_pq;
    std::vector<std::int64_t> m_neigh_distance;
    std::vector<std::int64_t> m_neigh_ids;
public:

    Dijkstra(const Structure &graph) : m_graph(graph) {
        m_neigh_distance = std::vector<std::int64_t>(graph.get_no_vertex(), std::numeric_limits<std::int64_t>::max());
        m_neigh_ids = std::vector<std::int64_t>(graph.get_no_vertex(), -1);
    };

    ~Dijkstra() = default;

    void compute(std::uint64_t src, std::uint64_t dst);
    [[nodiscard]] std::vector<std::int64_t> get_neigh() const;

};

#endif //DIJKSTRA_DIJKSTRA_H
