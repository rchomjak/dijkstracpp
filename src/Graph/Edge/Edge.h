//
// Created by a on 8/5/20.
//

#ifndef DIJKSTRA_EDGE_H
#define DIJKSTRA_EDGE_H

#include <memory>
#include "../Vertex/Vertex.h"

typedef std::uint64_t metric;
class Vertex;

class Edge {

private:
    std::shared_ptr<Vertex> m_end_1;
    std::shared_ptr<Vertex> m_end_2;
    metric m_metric;
    std::string m_description;
    std::uint64_t m_id;

public:
    void set_metric(metric in_metric){
        m_metric = in_metric;
    };
};


#endif //DIJKSTRA_EDGE_H
