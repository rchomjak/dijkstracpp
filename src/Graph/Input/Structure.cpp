//
// Created by a on 8/5/20.
//

#include "Structure.h"
#include <utility>
#include <regex>
#include <iterator>

Structure::graphState Structure::readData(std::string repr_graph) {

    auto automaton_state = m_input_automata.automatonMove;
    automaton_state->orig_input = std::move(repr_graph);

    while ((automaton_state->next = m_input_automata.m_automata_state_to_function.at(automaton_state->next)(
            automaton_state))
           && automaton_state->next != InputParseAutomaton::E_END_EPS);

    if (automaton_state->is_error) {
        return graphState::E_FAIL;
    }

    return graphState::E_LOADED;
}

bool Structure::connect_vertices(neighbour_edge_id_t first, neighbour_edge_id_t second, std::int64_t metric,
                                 std::string description) {

    try {
        std::shared_ptr<Vertex> first_vertex = m_id_vertex_map.at(first);
        std::shared_ptr<Vertex> second_vertex = m_id_vertex_map.at(second);
        auto edge = std::make_shared<Edge>(Edge(description, metric, first_vertex, second_vertex));

        first_vertex->add_edge(second, edge);
        second_vertex->add_edge(first, edge);

    } catch (const std::out_of_range &e) {
        return false;
    }

    return true;

}

neighbour_vertex_id_t Structure::get_no_vertex() const {
    return m_number_of_vertex;
}


void Structure::create_vertex(decltype(m_number_of_vertex) id) {
    m_id_vertex_map.insert(std::make_pair(id, std::make_shared<Vertex>(Vertex(id))));
    m_number_of_vertex++;
}

void Structure::delete_vertex(decltype(m_number_of_vertex) id) {

    try {
        auto vertex = m_id_vertex_map.at(id);
        for (auto &neigh: vertex->get_neighbours()) {
            neigh.second->del_edge(vertex->get_id());
        }

        vertex.reset();
        m_id_vertex_map.erase(id);
        m_number_of_vertex--;
    } catch (std::out_of_range &e) { ;
    }

}

std::shared_ptr<Vertex> Structure::get_vertex(neighbour_vertex_id_t id) const {
    return m_id_vertex_map.at(id);
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::init_automata_eps(std::shared_ptr<struct automatonMove> automaton) {

    std::regex re(R"((\(\s{0,}\d+\s{0,},\s{0,}\d{0,}\s{0,},\s{0,}\d+\s{0,},\s{0,}"\w{1,}"\s{0,}\))|[1-9][0-9]*)");
    std::copy(std::sregex_token_iterator(automaton->orig_input.begin(), automaton->orig_input.end(), re, 0),
              std::sregex_token_iterator(),
              std::back_inserter(automaton->iterator_token));

    automaton->iterator_token_state = automaton->iterator_token.begin();
    if (automaton->iterator_token.empty()) {
        //No token
        automaton->error_msg = "No token\r\n";
        automaton->is_error = true;
        return Structure::InputParseAutomaton::automatonState::E_AUTOMATA_ERROR_STATE;
    }

    return Structure::InputParseAutomaton::automatonState::E_NUMBER_NODES;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::number_nodes(std::shared_ptr<struct automatonMove> automaton) {

    auto in_val{std::strtoll(automaton->iterator_token_state->data(), nullptr, 10)};

    if (in_val < 0) {
        automaton->error_msg = "Incorrect input: " + std::to_string(in_val) + " \r\n";
        automaton->is_error = true;

        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    automaton->m_no_vertex = in_val;
    automaton->iterator_token_state++;

    if (automaton->iterator_token_state == automaton->iterator_token.end()) {
        automaton->error_msg = "Incorrect input: Missing data \r\n";
        automaton->is_error = true;
        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    for (size_t i = 0; i < automaton->m_no_vertex; i++) {
        this->m_parent.create_vertex(i);
    }

    return Structure::InputParseAutomaton::automatonState::E_NODE_CONNECTION;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::automata_error_state(std::shared_ptr<struct automatonMove> automaton) {
    std::cerr << automaton->error_msg << std::endl;
    return Structure::InputParseAutomaton::automatonState::E_END_EPS;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::get_connection(std::shared_ptr<struct automatonMove> automaton) {

    if (automaton->made_vertex >= automaton->m_no_vertex ||
        automaton->iterator_token_state == automaton->iterator_token.end()) {
        automaton->error_msg = "Incorrect input: Number of nodes do not correspond with connection lines.\r\n";
        automaton->is_error = true;
        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    std::smatch regexp_match;
    auto is_re_matched = std::regex_match(*automaton->iterator_token_state, regexp_match, automaton->connect_regex);

    if (not is_re_matched) {
        automaton->error_msg = "Incorrect input: regex " + *automaton->iterator_token_state + " \r\n";
        automaton->is_error = true;

        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    std::uint64_t src_node = std::strtoll(regexp_match[1].first.base(), nullptr, 10);
    std::uint64_t dst_node = std::strtoll(regexp_match[2].first.base(), nullptr, 10);
    std::uint64_t metric = std::strtoll(regexp_match[3].first.base(), nullptr, 10);
    std::string description = std::string(regexp_match[4].first.base());

    auto is_made = this->m_parent.connect_vertices(src_node - 1, dst_node - 1, metric, description);
    if (!is_made) {
        automaton->error_msg =
                "Incorrect input: Node does not have the connection" + *automaton->iterator_token_state + " \r\n";
        automaton->is_error = true;

        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    automaton->made_vertex++;
    automaton->iterator_token_state++;

    if (automaton->made_vertex == automaton->m_no_vertex) {
        return Structure::InputParseAutomaton::automatonState::E_END_EPS;
    }

    return Structure::InputParseAutomaton::automatonState::E_NODE_CONNECTION;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::end_automata_eps(std::shared_ptr<struct automatonMove> automaton) {
    return Structure::InputParseAutomaton::automatonState::E_END_EPS;
}
