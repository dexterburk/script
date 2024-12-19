#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <algorithm>
#include <string>
#include <map>
#include <vector>

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

static const int NUM_TERMINALS = 4;
static const int NUM_NON_TERMINALS = 5;
static const int NUM_STATES = 10;

static const std::map<std::string, int> terminalToID = {
    {"$", 3},
    {":", 1},
    {";", 2},
    {"IDENTIFIER", 0},
};

static const std::map<std::string, int> nonTerminalToID = {
    {"grammar", 0},
    {"identifier", 4},
    {"identifierList", 3},
    {"rule", 2},
    {"ruleList", 1},
};

static const std::vector<std::vector<Action>> actionTable = {
    { {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::SHIFT, 4}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::REDUCE, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 1}, },
    { {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::ACCEPT, -1}, },
    { {Action::SHIFT, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::REDUCE, 2}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 2}, },
    { {Action::SHIFT, 6}, {Action::NONE, -1}, {Action::REDUCE, 4}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 9}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 5}, {Action::NONE, -1}, },
    { {Action::REDUCE, 3}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 3}, },
};

static const std::vector<std::vector<Goto>> gotoTable = {
    { {-1}, {3}, {2}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {5}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {7}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {8}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, },
};

static const std::map<int, int> ruleSymbolCount = {
    {0, 1},
    {1, 1},
    {2, 2},
    {3, 4},
    {4, 1},
    {5, 2},
};

static const std::vector<Rule> grammar = {
    {"grammar", {"ruleList"}},
    {"ruleList", {"rule"}},
    {"ruleList", {"ruleList", "rule"}},
    {"rule", {"IDENTIFIER", ":", "identifierList", ";"}},
    {"identifierList", {"IDENTIFIER"}},
    {"identifierList", {"IDENTIFIER", "identifierList"}},
};

#endif // PARSER_HEADER
