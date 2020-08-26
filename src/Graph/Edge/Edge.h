//
// Created by a on 8/5/20.
//

#ifndef DIJKSTRA_EDGE_H
#define DIJKSTRA_EDGE_H

#include <memory>
#include <stdexcept>
#include "../Vertex/Vertex.h"

class Vertex;

class Edge {

public:
    typedef enum {
        E_END_1,
        E_END_2
    } direction_enum;

private:
    typedef std::uint64_t metric_t;

    std::weak_ptr<Vertex> m_end_1;
    std::weak_ptr<Vertex> m_end_2;
    metric_t m_metric{0};
    std::string m_description;

public:

    void set_metric(metric_t in_metric);

    metric_t get_metric() const;

    void set_description(std::string in_description);

    decltype(m_description) get_description() const;

    friend std::ostream &operator<<(std::ostream &out, const Edge &edge);

    Edge() {};

    ~Edge() = default;

    Edge(metric_t metric) : m_metric(metric) {};

    Edge(std::string in_description) : m_description(in_description) {};

    Edge(std::string in_description, metric_t metric) : m_metric(metric), m_description(in_description) {};

    Edge(std::string in_description, metric_t metric, std::weak_ptr<Vertex> vertex_src,
         std::weak_ptr<Vertex> vertex_dst) : m_metric(metric), m_description(in_description) {
        make_connection(vertex_src, vertex_dst);
    };

    void make_connection(std::weak_ptr<Vertex> vertex_src, std::weak_ptr<Vertex> vertex_dst);

    std::vector<std::weak_ptr<Vertex>> get_vertices();


private:
    void make_connection(Edge::direction_enum direction, std::weak_ptr<Vertex> vertex);

};


#endif //DIJKSTRA_EDGE_H
