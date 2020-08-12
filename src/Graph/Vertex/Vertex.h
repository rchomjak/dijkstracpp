//
// Created by a on 8/5/20.
//

#ifndef DIJKSTRA_VERTEX_H
#define DIJKSTRA_VERTEX_H

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

#include "../Edge/Edge.h"

typedef std::int64_t neighbour_edge_id;
typedef std::int64_t neighbour_vertex_id;

class Edge;
class Vertex {

private:
    neighbour_vertex_id m_id {-1};
    std::unordered_map<neighbour_edge_id, std::shared_ptr<Edge>> m_edges_ptr_map;

public:
    //TODO: del specific connection
    //TODO: add specific connection
    //TODO: get neighbours

    Vertex() {};
    ~Vertex() = default;

    Vertex(neighbour_vertex_id id): m_id(id){};

    void set_id(neighbour_vertex_id vertex_id){
        m_id = vertex_id;
    }

    bool add_edge(neighbour_edge_id edge_id, std::shared_ptr<Edge> edge) {

        m_edges_ptr_map.insert({edge_id, edge});
    }

};

#endif //DIJKSTRA_VERTEX_H
