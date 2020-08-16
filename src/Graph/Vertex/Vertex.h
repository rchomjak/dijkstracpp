//
// Created by a on 8/5/20.
//

#ifndef DIJKSTRA_VERTEX_H
#define DIJKSTRA_VERTEX_H

#include <atomic>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

#include "../Edge/Edge.h"

typedef std::int64_t neighbour_edge_id_t;
typedef std::int64_t neighbour_vertex_id_t;

class Edge;

class Vertex {

private:
    neighbour_vertex_id_t m_id;
    std::unordered_map<neighbour_edge_id_t, std::shared_ptr<Edge>> m_edges_ptr_map;

public:

    Vertex() {};
    ~Vertex() = default;

    Vertex(std::int64_t id): m_id(id){};

    void set_id(decltype(Vertex::m_id) vertex_id);
    bool add_edge(neighbour_edge_id_t edge_id, std::shared_ptr<Edge> edge);
    void del_edge(neighbour_edge_id_t edge_id);
    std::vector<std::shared_ptr<Edge>> get_edge(neighbour_edge_id_t edge_id) const;
    std::vector<std::shared_ptr<Edge>> get_edges() const;
    std::vector<std::pair<std::int64_t , std::shared_ptr<Vertex>>> get_neighbours() const;


    friend std::ostream& operator<<(std::ostream &out, const Vertex &vertex);
    neighbour_vertex_id_t get_id() const;

};



#endif //DIJKSTRA_VERTEX_H
