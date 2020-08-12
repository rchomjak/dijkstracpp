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
 * N_1 - N_1_connections_with_metrics_description in the order (node, metric, description)
 * N_2 - N_2_connections_with_metrics_description
 * -
 * N_4 - N_4_connections_with_metrics_and_description
 * ...
 * ---
 * Example:
 *
 * 3
 * (2, 100m "Connection from first node to second"), (3,10, "Connection from first node to second")
 * - #NOTE the second node does not have any connections
 * (2, 200,"") (1, 100, ""),
 *
 **/


class Structure {

private:

    std::string m_repr_graph;
    std::ifstream m_file_repr_graph;

    std::unordered_map<neighbour_vertex_id, std::shared_ptr<Vertex>> m_id_vertex_map;

    void create_vertex(neighbour_vertex_id id) {
      m_id_vertex_map.insert(std::make_pair(id,std::make_shared<Vertex>(Vertex(id))));
    }

public:

    Structure():m_input_automata(*this) {};
    ~Structure() {};

    void readData(std::string repr_graph);

private:
    class InputParseAutomaton {
        friend class Structure;

    public:
        typedef enum automatonState {
            E_INIT_EPS, // Possibly to use as init state
            E_NUMBER_NODES, // Returns number of nodes in the file
            E_NODE_CONNECTION, // Returns representation of
            E_AUTOMATA_ERROR_STATE, // Error in file
            E_END_EPS // End of parsing
        } automataState;

    private:
        struct automatonMove  {
            std::uint64_t m_no_vertex{0};
            std::uint64_t made_vertex{0};
            Structure::InputParseAutomaton::automatonState next = InputParseAutomaton::E_INIT_EPS;
            std::string orig_input;
            std::regex connect_regex = std::regex(R"(\(\s{0,}(\d+)\s{0,},\s{0,}(\d+)\s{0,},\s{0,}(\w{0,})\s{0,}\))");
            std::vector<std::string> iterator_token;
            std::vector<std::string>::iterator iterator_token_state;
            std::string error_msg;
        };

        std::shared_ptr<struct automatonMove> automatonMove =  std::make_shared<struct automatonMove>();
        Structure &m_parent;

    public:

        InputParseAutomaton(Structure &outer_struct): m_parent(outer_struct) {};
        ~InputParseAutomaton() = default;

        automatonState init_automata_eps( std::shared_ptr<struct automatonMove>);
        automatonState number_nodes( std::shared_ptr<struct automatonMove>);
        automatonState get_connection( std::shared_ptr<struct automatonMove>);
        automatonState automata_error_state( std::shared_ptr<struct automatonMove>);
        automatonState end_automata_eps( std::shared_ptr<struct automatonMove>);

        std::map<automatonState, std::function<automatonState(  std::shared_ptr<struct automatonMove>)>> m_automata_state_to_function {
                {E_INIT_EPS, [this](  std::shared_ptr<struct automatonMove> x) -> automatonState {return init_automata_eps(x);}},
                {E_NUMBER_NODES, [this](  std::shared_ptr<struct automatonMove> x) -> automatonState {return number_nodes(x);}},
                {E_NODE_CONNECTION, [this](  std::shared_ptr<struct automatonMove> x) -> automatonState {return get_connection(x);}},
                {E_AUTOMATA_ERROR_STATE, [this](  std::shared_ptr<struct automatonMove> x) -> automatonState {return automata_error_state(x);}},
                {E_END_EPS, [this](  std::shared_ptr<struct automatonMove> x) -> automatonState {return end_automata_eps(x);}}

        };
    };
        InputParseAutomaton m_input_automata;
    };


#endif //DIJKSTRA_STRUCTURE_H
