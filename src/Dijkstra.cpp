//
// Created by a on 8/16/20.
//

#include "Dijkstra.h"

void Dijkstra::compute(std::uint64_t src, std::uint64_t dst) {

    // src, dst >= 0
    auto src_vertex = m_graph.get_vertex(src);
    auto dst_vertex = m_graph.get_vertex(dst);

    m_pq.push({0, src_vertex});
    m_neigh_distance.at(src) = 0;

    while (!m_pq.empty()) {
        auto current_vertex = m_pq.top();
        m_pq.pop();
        for (const auto &neigh: current_vertex.second->get_neighbours()) {

            if (current_vertex.first + neigh.first < m_neigh_distance.at(neigh.second->get_id())) {
                m_neigh_distance.at(neigh.second->get_id()) = current_vertex.first + neigh.first;
                m_pq.push({(current_vertex.first + neigh.first), neigh.second});
                m_neigh_ids.at(neigh.second->get_id()) = current_vertex.second->get_id();
            }

        }
    }

    return;

}

std::vector<std::int64_t> Dijkstra::get_neigh() const {
    return m_neigh_ids;
}