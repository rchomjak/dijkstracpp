//
// Created by a on 8/5/20.
//

#include "Edge.h"
std::ostream& operator<<(std::ostream &out, const Edge &edge) {

    out << "Edge: ";
    out << " Description: "<<edge.get_description();
    out << ", Metric: "<< edge.get_metric();
    out << ", End_1: "<< ((edge.m_end_1.expired()) ? -1337: (edge.m_end_1.lock())->get_id());
    out << ", End_2: "<< ((edge.m_end_2.expired()) ? -1337 : (edge.m_end_2.lock())->get_id());
    return out;
}

void Edge::set_metric(metric_t in_metric) {
    m_metric = in_metric;
}

Edge::metric_t Edge::get_metric() const{
    return m_metric;
}

void Edge::set_description(std::string in_description) {
    m_description = in_description;
}

decltype(Edge::m_description) Edge::get_description() const {
    return m_description;
}

void Edge::make_connection(std::weak_ptr<Vertex> vertex_src, std::weak_ptr<Vertex> vertex_dst) {
    make_connection(Edge::direction_enum::E_END_1, vertex_src);
    make_connection(Edge::direction_enum::E_END_2, vertex_dst);
}

std::vector<std::weak_ptr<Vertex>> Edge::get_vertices() {
    return {m_end_1, m_end_2};
}

void Edge::make_connection(Edge::direction_enum direction, std::weak_ptr<Vertex> vertex) {

    if (!m_end_1.lock() && direction == Edge::direction_enum::E_END_1) {
        m_end_1 = vertex;
        return;

    } else if (!m_end_2.lock() && direction == Edge::direction_enum::E_END_2) {
        m_end_2 = vertex;
        return;
    }

    throw std::logic_error("ERROR: Connection is already exists.  \r\n");
}