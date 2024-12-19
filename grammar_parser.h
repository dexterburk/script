#ifndef PARSER_H
#define PARSER_H

#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace GrammarParser {

#include "grammar_cst.h"

struct Action {
    enum ActionType {
        SHIFT,
        REDUCE,
        ACCEPT,
        NONE
    };
    ActionType actionType;
    int stateOrRule; // For SHIFT: state; For REDUCE: rule index
};

struct Goto {
    int state;  // The state to go to for a non-terminal
};

struct Rule {
    std::string lhs;  // Left-hand side of the rule
    std::vector<std::string> rhs;  // Right-hand side of the rule (sequence of symbols)
};

static const int NUM_TERMINALS = 5;
static const int NUM_NON_TERMINALS = 6;
static const int NUM_STATES = 14;

static const std::map<std::string, int> terminalToID = {
    {"COLON", 1},
    {"END_OF_FILE", 4},
    {"IDENTIFIER", 0},
    {"SEMICOLON", 2},
    {"VERTICAL_BAR", 3},
};

static const std::map<std::string, int> nonTerminalToID = {
    {"grammar", 0},
    {"identifierList", 5},
    {"option", 4},
    {"optionList", 3},
    {"rule", 2},
    {"ruleList", 1},
};

static const std::vector<std::vector<Action>> actionTable = {
    { {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::SHIFT, 4}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::REDUCE, 2}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 2}, },
    { {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::ACCEPT, -1}, },
    { {Action::SHIFT, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::REDUCE, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 1}, },
    { {Action::REDUCE, 8}, {Action::NONE, -1}, {Action::REDUCE, 8}, {Action::REDUCE, 8}, {Action::NONE, -1}, },
    { {Action::SHIFT, 10}, {Action::NONE, -1}, {Action::REDUCE, 6}, {Action::REDUCE, 6}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 5}, {Action::REDUCE, 5}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 11}, {Action::SHIFT, 12}, {Action::NONE, -1}, },
    { {Action::REDUCE, 7}, {Action::NONE, -1}, {Action::REDUCE, 7}, {Action::REDUCE, 7}, {Action::NONE, -1}, },
    { {Action::REDUCE, 3}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 3}, },
    { {Action::SHIFT, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 4}, {Action::REDUCE, 4}, {Action::NONE, -1}, },
};

static const std::vector<std::vector<Goto>> gotoTable = {
    { {-1}, {3}, {2}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {5}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {9}, {8}, {7}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {13}, {7}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
};

static const std::map<int, int> ruleSymbolCount = {
    {0, 1},
    {1, 2},
    {2, 1},
    {3, 4},
    {4, 3},
    {5, 1},
    {6, 1},
    {7, 2},
    {8, 1},
};

static const std::vector<Rule> grammar = {
    {"grammar", {"ruleList"}},
    {"ruleList", {"ruleList", "rule"}},
    {"ruleList", {"rule"}},
    {"rule", {"IDENTIFIER", "COLON", "optionList", "SEMICOLON"}},
    {"optionList", {"optionList", "VERTICAL_BAR", "option"}},
    {"optionList", {"option"}},
    {"option", {"identifierList"}},
    {"identifierList", {"identifierList", "IDENTIFIER"}},
    {"identifierList", {"IDENTIFIER"}},
};

std::string readFile(const std::string &filename);
std::vector<CSTNode *> tokenize(const std::string &input);
CSTNode* parse(const std::vector<CSTNode *>& input);

};

#endif // PARSER_H
