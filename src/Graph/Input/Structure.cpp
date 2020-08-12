//
// Created by a on 8/5/20.
//
#include "Structure.h"

#include <utility>
#include <regex>
#include <iterator>

void Structure::readData(std::string repr_graph) {

    auto automaton_state = m_input_automata.automatonMove;
    automaton_state->orig_input = std::move(repr_graph);

    while ((automaton_state->next = m_input_automata.m_automata_state_to_function.at(automaton_state->next)(
            automaton_state))
           && automaton_state->next != InputParseAutomaton::E_END_EPS);
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::init_automata_eps( std::shared_ptr<struct automatonMove> automaton) {

    std::regex re("(\\(\\s{0,}\\d+\\s{0,},\\s{0,}\\d+\\s{0,},\\s{0,}\\w{1,}\\s{0,}\\))|[1-9][0-9]*");

    std::copy(std::sregex_token_iterator(automaton->orig_input.begin(), automaton->orig_input.end(), re, 0),
              std::sregex_token_iterator(),
              std::back_inserter(automaton->iterator_token));

    automaton->iterator_token_state = automaton->iterator_token.begin();
    if (automaton->iterator_token.size() == 0) {
        //No token
        automaton->error_msg = "No token\r\n";
        return Structure::InputParseAutomaton::automatonState::E_AUTOMATA_ERROR_STATE;
    }

    return Structure::InputParseAutomaton::automatonState::E_NUMBER_NODES;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::number_nodes(std::shared_ptr<struct automatonMove> automaton) {

    auto in_val {std::strtoll(automaton->iterator_token_state->data(), nullptr, 10)};

    if (in_val < 0) {
        automaton->error_msg = "Incorrect input: " + std::to_string(in_val) + " \r\n";
        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    automaton->m_no_vertex = in_val;
    automaton->iterator_token_state++;

    if (automaton->iterator_token_state == automaton->iterator_token.end()) {
        automaton->error_msg = "Incorrect input: Missing data \r\n";
        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    //TODO: make N new nodes

    for (size_t i = 0 ; i < automaton->m_no_vertex; i++) {
        this->m_parent.create_vertex(i);
    }

    return  Structure::InputParseAutomaton::automatonState::E_NODE_CONNECTION;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::automata_error_state( std::shared_ptr<struct automatonMove> automaton) {
    std::cerr<<automaton->error_msg<<std::endl;
    return Structure::InputParseAutomaton::automatonState::E_END_EPS;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::get_connection( std::shared_ptr<struct automatonMove> automaton) {



    if (automaton->made_vertex >= automaton->m_no_vertex) {
        automaton->error_msg = "Incorrect input: Number of nodes do not correspond with connection lines.\r\n";
        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    std::smatch regexp_match;
    auto is_re_matched = std::regex_match(*automaton->iterator_token_state,regexp_match, automaton->connect_regex);

    if (not is_re_matched) {
        automaton->error_msg = "Incorrect input: " + *automaton->iterator_token_state + " \r\n";
        return Structure::InputParseAutomaton::automataState::E_AUTOMATA_ERROR_STATE;
    }

    std::uint64_t connect_to = std::strtoll(regexp_match[1].first.base(), nullptr, 10);
    std::uint64_t metric = std::strtoll(regexp_match[2].first.base(), nullptr, 10);
    std::string description = regexp_match[3].first.base();

    // TODO: Create new node with connection

    return Structure::InputParseAutomaton::automatonState::E_END_EPS;
}

Structure::InputParseAutomaton::automatonState
Structure::InputParseAutomaton::end_automata_eps( std::shared_ptr<struct automatonMove> automaton) {
    return Structure::InputParseAutomaton::automatonState::E_END_EPS;

}


