//
// Created by a on 8/5/20.
//

#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>

#include "../Edge/Edge.h"
#include "Vertex.h"


void Vertex::set_id(decltype(Vertex::m_id) vertex_id) {
    m_id = vertex_id;
}

bool Vertex::add_edge(neighbour_edge_id_t edge_id, std::shared_ptr<Edge> edge) {
    //do not want to return from try/catch block, In c++20 it could be substitute with contains
    bool in_map = true;

    try {
        m_edges_ptr_map.at(edge_id);
        in_map = false;

    } catch (const std::out_of_range &exception) {
        m_edges_ptr_map.insert({edge_id, edge});
    }

    return in_map;
}

void Vertex::del_edge(neighbour_edge_id_t edge_id) {
    m_edges_ptr_map.erase(edge_id);
}

std::vector<std::shared_ptr<Edge>> Vertex::get_edge(neighbour_edge_id_t edge_id) const {

    std::vector<std::shared_ptr<Edge>> return_edge_vec;

    try {
        return_edge_vec.emplace_back(m_edges_ptr_map.at(edge_id));
    } catch (const std::out_of_range &exception) { ;
    }

    return return_edge_vec;
}

std::vector<std::pair<std::int64_t, std::shared_ptr<Vertex>>> Vertex::get_neighbours() const {

    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Edge> edge;
    std::int64_t metric;
    std::vector<std::pair<std::int64_t, std::shared_ptr<Vertex>>> return_vector;

    bool am_i_in_edge = false;

    std::weak_ptr<Vertex> neigh_ptr;

    for (auto &edge_neighbour: m_edges_ptr_map) {

        am_i_in_edge = false;

        edge = edge_neighbour.second;
        metric = edge->get_metric();

        for (auto &l_vertex: edge->get_vertices()) {
            if (!l_vertex.expired()) {
                auto ptr = l_vertex.lock();
                if (ptr.get() == this) {
                    am_i_in_edge = true;
                } else {
                    neigh_ptr = ptr;
                }
            }
        }

        if (!am_i_in_edge) {
            throw std::logic_error("ERROR, graph is not valid, local edge without friend mine (this).\r\n");
        }
        if (!neigh_ptr.expired()) {
            return_vector.emplace_back(metric, neigh_ptr.lock());
        }
    }

    return return_vector;
}

neighbour_vertex_id_t Vertex::get_id() const {
    return m_id;
}

std::vector<std::shared_ptr<Edge>> Vertex::get_edges() const {
    auto ret_vec = std::vector<std::shared_ptr<Edge>>();
    for (auto &edge: m_edges_ptr_map) {
        ret_vec.emplace_back(edge.second);
    }
    return ret_vec;
}

std::ostream &operator<<(std::ostream &out, const Vertex &vertex) {
    out << "{Vertex id: " << std::to_string(vertex.get_id());
    out << ", Edges:{";
    for (auto &edge: vertex.get_edges()) {
        out << "{";
        out << *edge << "}, ";
    }
    out << "}}";
    return out;
}