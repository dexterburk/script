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

static const int NUM_TERMINALS = 15;
static const int NUM_NON_TERMINALS = 12;
static const int NUM_STATES = 39;

static const std::map<std::string, int> terminalToID = {
    {"$", 14},
    {"ASTERISK", 11},
    {"COMMA", 6},
    {"IDENTIFIER", 0},
    {"INT", 5},
    {"LEFT_BRACE", 3},
    {"LEFT_PARENTHESIS", 1},
    {"MINUS", 10},
    {"NUMBER", 13},
    {"PLUS", 9},
    {"RETURN", 7},
    {"RIGHT_BRACE", 4},
    {"RIGHT_PARENTHESIS", 2},
    {"SEMICOLON", 8},
    {"SLASH", 12},
};

static const std::map<std::string, int> nonTerminalToID = {
    {"expression", 9},
    {"factor", 11},
    {"function", 2},
    {"functionList", 1},
    {"parameter", 5},
    {"parameterList", 4},
    {"program", 0},
    {"returnStatement", 8},
    {"statement", 7},
    {"statementList", 6},
    {"term", 10},
    {"type", 3},
};

static const std::vector<std::vector<Action>> actionTable = {
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::REDUCE, 4}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 2}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 2}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::ACCEPT, -1}, },
    { {Action::SHIFT, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 1}, },
    { {Action::NONE, -1}, {Action::SHIFT, 7}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 12}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 11}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::SHIFT, 13}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 15}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 7}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 7}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 5}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 5}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 16}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::SHIFT, 20}, {Action::SHIFT, 21}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 22}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 10}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 10}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 9}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 9}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 26}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 16}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 19}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 19}, {Action::REDUCE, 19}, {Action::REDUCE, 19}, {Action::REDUCE, 19}, {Action::REDUCE, 19}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::SHIFT, 20}, {Action::SHIFT, 21}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 22}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 20}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 20}, {Action::REDUCE, 20}, {Action::REDUCE, 20}, {Action::REDUCE, 20}, {Action::REDUCE, 20}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 31}, {Action::SHIFT, 30}, {Action::SHIFT, 29}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 17}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 17}, {Action::REDUCE, 17}, {Action::REDUCE, 17}, {Action::REDUCE, 17}, {Action::REDUCE, 17}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 14}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 14}, {Action::REDUCE, 14}, {Action::REDUCE, 14}, {Action::SHIFT, 32}, {Action::SHIFT, 33}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 3}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 3}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 8}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 8}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 34}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 30}, {Action::SHIFT, 29}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::SHIFT, 20}, {Action::SHIFT, 21}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 22}, {Action::NONE, -1}, },
    { {Action::SHIFT, 20}, {Action::SHIFT, 21}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 22}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 11}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 11}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::SHIFT, 20}, {Action::SHIFT, 21}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 22}, {Action::NONE, -1}, },
    { {Action::SHIFT, 20}, {Action::SHIFT, 21}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 22}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 18}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 18}, {Action::REDUCE, 18}, {Action::REDUCE, 18}, {Action::REDUCE, 18}, {Action::REDUCE, 18}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 13}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 13}, {Action::REDUCE, 13}, {Action::REDUCE, 13}, {Action::SHIFT, 32}, {Action::SHIFT, 33}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 12}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 12}, {Action::REDUCE, 12}, {Action::REDUCE, 12}, {Action::SHIFT, 32}, {Action::SHIFT, 33}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 15}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 15}, {Action::REDUCE, 15}, {Action::REDUCE, 15}, {Action::REDUCE, 15}, {Action::REDUCE, 15}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 16}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 16}, {Action::REDUCE, 16}, {Action::REDUCE, 16}, {Action::REDUCE, 16}, {Action::REDUCE, 16}, {Action::NONE, -1}, {Action::NONE, -1}, },
};

static const std::vector<std::vector<Goto>> gotoTable = {
    { {-1}, {3}, {2}, {4}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {5}, {4}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {10}, {9}, {8}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {10}, {-1}, {14}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {19}, {18}, {17}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {23}, {25}, {24}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {27}, {17}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {28}, {25}, {24}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {35}, {24}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {36}, {24}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {37}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {38}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, },
};

static const std::map<int, int> ruleSymbolCount = {
    {0, 1},
    {1, 2},
    {2, 1},
    {3, 8},
    {4, 1},
    {5, 3},
    {6, 1},
    {7, 2},
    {8, 2},
    {9, 1},
    {10, 1},
    {11, 3},
    {12, 3},
    {13, 3},
    {14, 1},
    {15, 3},
    {16, 3},
    {17, 1},
    {18, 3},
    {19, 1},
    {20, 1},
};

static const std::vector<Rule> grammar = {
    {"program", {"functionList"}},
    {"functionList", {"functionList", "function"}},
    {"functionList", {"function"}},
    {"function", {"type", "IDENTIFIER", "LEFT_PARENTHESIS", "parameterList", "RIGHT_PARENTHESIS", "LEFT_BRACE", "statementList", "RIGHT_BRACE"}},
    {"type", {"INT"}},
    {"parameterList", {"parameterList", "COMMA", "parameter"}},
    {"parameterList", {"parameter"}},
    {"parameter", {"type", "IDENTIFIER"}},
    {"statementList", {"statementList", "statement"}},
    {"statementList", {"statement"}},
    {"statement", {"returnStatement"}},
    {"returnStatement", {"RETURN", "expression", "SEMICOLON"}},
    {"expression", {"expression", "PLUS", "term"}},
    {"expression", {"expression", "MINUS", "term"}},
    {"expression", {"term"}},
    {"term", {"term", "ASTERISK", "factor"}},
    {"term", {"term", "SLASH", "factor"}},
    {"term", {"factor"}},
    {"factor", {"LEFT_PARENTHESIS", "expression", "RIGHT_PARENTHESIS"}},
    {"factor", {"IDENTIFIER"}},
    {"factor", {"NUMBER"}},
};

#endif // PARSER_HEADER
