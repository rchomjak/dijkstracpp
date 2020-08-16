//
// Created by a on 8/5/20.
//

#ifndef DIJKSTRA_STRUCTURE_H
#define DIJKSTRA_STRUCTURE_H

#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <unordered_map>

#include <regex>
#include "../Vertex/Vertex.h"

/**
 * WARNING: EXPECT, THAT FORMAT INPUT IS IN CORRECT FORM
 * Connections are unidirectional, vertex can have multiple connections to a neighbour.
 * Structure of the input is following:
 *
 * n - number of nodes
 * N_1 - N_1_connections_with_metrics_description in the order (node_src, node_dst, metric, description)
 * N_2 - N_2_connections_with_metrics_description
 * -
 * N_4 - N_4_connections_with_metrics_and_description
 * ...
 * ---
 * Example:
 *
 * 3
 * (1, 2, 100m "Connection from first node to second"), (1,3,10, "Connection from first node to second")
 * - #NOTE the second node does not have any connections
 * (2, , 200,"") (1, ,2, 100, ""),
 *
 **/


class Structure {


public:
    typedef enum graphState {
        E_LOADED = EXIT_SUCCESS,
        E_FAIL
    } graphState;

private:
    neighbour_vertex_id_t m_number_of_vertex{0};

    std::string m_repr_graph;
    std::ifstream m_file_repr_graph;

    std::unordered_map<decltype(m_number_of_vertex), std::shared_ptr<Vertex>> m_id_vertex_map;

    bool connect_vertices(neighbour_edge_id_t first, neighbour_edge_id_t second, std::int64_t metric,
                          std::string description);

public:

    friend std::ostream &operator<<(std::ostream &os, const Structure &obj) {

        os << "[";

        for (const auto &x: obj.m_id_vertex_map) {
            os << "{";
            auto &vex = x.second;
            os << *vex << "";

            for (auto &edge: vex.get()->get_edges()) {
                os << "{";
                os << *edge;
                os << "},";
            }
            os << "},";
        }
        os << "]";
        return os;
    }


    Structure() : m_input_automata(*this) {};

    ~Structure() {};

    graphState readData(std::string repr_graph);

    neighbour_vertex_id_t get_no_vertex() const;

    void create_vertex(decltype(m_number_of_vertex) id);

    void delete_vertex(decltype(m_number_of_vertex) id);

    std::shared_ptr<Vertex> get_vertex(decltype(m_number_of_vertex) id) const;


private:
    class InputParseAutomaton {
        friend class Structure;

    public:
        typedef enum automatonState {
            E_INIT_EPS, // Possibly to use as init state
            E_NUMBER_NODES, // Makes nodes number of nodes in the file
            E_NODE_CONNECTION, // Makes nodes
            E_AUTOMATA_ERROR_STATE, // Error in file
            E_END_EPS // End of parsing
        } automataState;

    private:
        struct automatonMove {
            std::uint64_t m_no_vertex{0};
            std::uint64_t made_vertex{0};
            Structure::InputParseAutomaton::automatonState next = InputParseAutomaton::E_INIT_EPS;
            std::string orig_input;
            std::regex connect_regex = std::regex(
                    R"(\(\s*?(\d+?)\s*?,\s*?(\d+?)\s*?,\s*?(\d+)\s*?,\s*?\"(\w*?)\"\s*?\))");
            std::vector<std::string> iterator_token;
            std::vector<std::string>::iterator iterator_token_state;
            bool is_error = false;
            std::string error_msg;
        };

        std::shared_ptr<struct automatonMove> automatonMove = std::make_shared<struct automatonMove>();
        Structure &m_parent;

    public:

        InputParseAutomaton(Structure &outer_struct) : m_parent(outer_struct) {};

        ~InputParseAutomaton() = default;

        automatonState init_automata_eps(std::shared_ptr<struct automatonMove>);

        automatonState number_nodes(std::shared_ptr<struct automatonMove>);

        automatonState get_connection(std::shared_ptr<struct automatonMove>);

        automatonState automata_error_state(std::shared_ptr<struct automatonMove>);

        automatonState end_automata_eps(std::shared_ptr<struct automatonMove>);

        std::map<automatonState, std::function<automatonState(
                std::shared_ptr<struct automatonMove>)>> m_automata_state_to_function{
                {E_INIT_EPS,             [this](
                        std::shared_ptr<struct automatonMove> x) -> automatonState { return init_automata_eps(x); }},
                {E_NUMBER_NODES,         [this](
                        std::shared_ptr<struct automatonMove> x) -> automatonState { return number_nodes(x); }},
                {E_NODE_CONNECTION,      [this](
                        std::shared_ptr<struct automatonMove> x) -> automatonState { return get_connection(x); }},
                {E_AUTOMATA_ERROR_STATE, [this](
                        std::shared_ptr<struct automatonMove> x) -> automatonState { return automata_error_state(x); }},
                {E_END_EPS,              [this](
                        std::shared_ptr<struct automatonMove> x) -> automatonState { return end_automata_eps(x); }}

        };
    };

    InputParseAutomaton m_input_automata;
};


#endif //DIJKSTRA_STRUCTURE_H
