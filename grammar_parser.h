#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <sstream>
#include <stdexcept>

namespace GrammarParser {

using namespace std;

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
    {"$", 4},
    {"COLON", 1},
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
    { {Action::REDUCE, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 1}, },
    { {Action::SHIFT, 1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::ACCEPT, -1}, },
    { {Action::SHIFT, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::REDUCE, 2}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 2}, },
    { {Action::REDUCE, 8}, {Action::NONE, -1}, {Action::REDUCE, 8}, {Action::REDUCE, 8}, {Action::NONE, -1}, },
    { {Action::SHIFT, 10}, {Action::NONE, -1}, {Action::REDUCE, 6}, {Action::REDUCE, 6}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 4}, {Action::REDUCE, 4}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::SHIFT, 11}, {Action::SHIFT, 12}, {Action::NONE, -1}, },
    { {Action::REDUCE, 7}, {Action::NONE, -1}, {Action::REDUCE, 7}, {Action::REDUCE, 7}, {Action::NONE, -1}, },
    { {Action::REDUCE, 3}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 3}, },
    { {Action::SHIFT, 6}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, {Action::NONE, -1}, },
    { {Action::NONE, -1}, {Action::NONE, -1}, {Action::REDUCE, 5}, {Action::REDUCE, 5}, {Action::NONE, -1}, },
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
    {1, 1},
    {2, 2},
    {3, 4},
    {4, 1},
    {5, 3},
    {6, 1},
    {7, 2},
    {8, 1},
};

static const std::vector<Rule> grammar = {
    {"grammar", {"ruleList"}},
    {"ruleList", {"rule"}},
    {"ruleList", {"ruleList", "rule"}},
    {"rule", {"IDENTIFIER", "COLON", "optionList", "SEMICOLON"}},
    {"optionList", {"option"}},
    {"optionList", {"optionList", "VERTICAL_BAR", "option"}},
    {"option", {"identifierList"}},
    {"identifierList", {"identifierList", "IDENTIFIER"}},
    {"identifierList", {"IDENTIFIER"}},
};

struct ASTNode {
    string symbol;  // The symbol of this node (terminal or non-terminal)
    vector<ASTNode*> children;  // The children of this node (for non-terminals)
    
    ASTNode(const string& symbol) : symbol(symbol) {}

    // Function to add a child node
    void addChild(ASTNode* child) {
        children.push_back(child);
    }

    // Function to print the tree (for debugging purposes)
    void print(int level = 0) const {
        for (int i = 0; i < level; ++i) cout << "  ";  // Indentation for depth
        cout << symbol << endl;
        for (auto* child : children) {
            child->print(level + 1);
        }
    }
};

// The LR(1) parser function
ASTNode* parse(const vector<ASTNode *>& input) {
    stack<int> stateStack;  // Stack to store states
    stack<ASTNode*> astStack;  // Stack to store AST nodes for each symbol
    
    stateStack.push(0);  // Initial state is 0
    
    size_t inputIndex = 0;  // Pointer to the current symbol in the input
    ASTNode *currentSymbol = input[inputIndex];  // Current symbol to process
    
    while (true) {
        int currentState = stateStack.top();  // Top of the state stack
        cout << "Current State: " << currentState << ", Current Symbol: " << currentSymbol->symbol << endl;

        // Get the action for the current state and symbol
        Action currentAction = actionTable[currentState][terminalToID.at(currentSymbol->symbol)];

        // Handle the action
        switch (currentAction.actionType) {
            case Action::SHIFT: {
                // Perform shift: push the new state and create an AST node for the symbol
                cout << "Action: SHIFT, Next State: " << currentAction.stateOrRule << endl;
                stateStack.push(currentAction.stateOrRule);  // Shift to the new state
                astStack.push(currentSymbol);  // Push the AST node onto the stack
                inputIndex++;  // Move to the next symbol in the input
                if (inputIndex < input.size()) {
                    currentSymbol = input[inputIndex];  // Update current symbol
                } else {
                    currentSymbol = new ASTNode("$");
                }
                break;
            }

            case Action::REDUCE: {
                // Perform reduce: pop symbols and states according to the rule
                int ruleIndex = currentAction.stateOrRule;
                const Rule& rule = grammar[ruleIndex];
                cout << "Action: REDUCE by rule " << ruleIndex << ": " << rule.lhs << " -> ";
                for (const string& symbol : rule.rhs) {
                    cout << symbol << " ";
                }
                cout << endl;

                // Pop symbols and states from the stacks
                vector<ASTNode*> rhsNodes;
                for (size_t i = 0; i < rule.rhs.size(); ++i) {
                    stateStack.pop();
                    ASTNode* rhsNode = astStack.top();
                    astStack.pop();
                    rhsNodes.push_back(rhsNode);  // Collect the AST nodes for the RHS symbols
                }

                // Reverse the order of the RHS nodes
                reverse(rhsNodes.begin(), rhsNodes.end());

                // Create a new AST node for the left-hand side (LHS) of the rule
                ASTNode* parentNode = new ASTNode(rule.lhs);

                // Add the RHS nodes as children of the parent node
                for (auto* rhsNode : rhsNodes) {
                    parentNode->addChild(rhsNode);
                }

                // Get the state to go to from the goto table
                int nextState = gotoTable[stateStack.top()][nonTerminalToID.at(rule.lhs)].state;
                cout << "Goto state: " << nextState << endl;

                // Push the non-terminal and the new state onto the stack
                stateStack.push(nextState);
                astStack.push(parentNode);  // Push the parent node onto the AST stack

                break;
            }

            case Action::ACCEPT:
                cout << "Action: ACCEPT. Parsing is complete!" << endl;
                return astStack.top();

            case Action::NONE:
                throw runtime_error("Parsing error: No action available.");
        }
    }
}

vector<ASTNode*> tokenize(const string& input) {
    vector<ASTNode*> tokens;
    int index = 0;
    while (index < input.length()) {
        if (isspace(input[index])) {
            index++;
            continue;
        }
        // Create ASTNode for single character tokens
        if (input[index] == ':') {
            tokens.push_back(new ASTNode("COLON"));
            index++;
            continue;
        }
        if (input[index] == ';') {
            tokens.push_back(new ASTNode("SEMICOLON"));
            index++;
            continue;
        }
        if (input[index] == '|') {
            tokens.push_back(new ASTNode("VERTICAL_BAR"));
            index++;
            continue;
        }
        // Handle identifiers
        if (isalpha(input[index]) || input[index] == '_') {
            string identifier;
            while (index < input.length() && (isalnum(input[index]) || input[index] == '_')) {
                identifier += input[index++];
            }
            ASTNode* identifierNode = new ASTNode("IDENTIFIER");
            identifierNode->addChild(new ASTNode(identifier));
            tokens.push_back(identifierNode);
            continue;
        }
        throw runtime_error("Unknown token: " + string(1, input[index]));
    }
    // Add end of input symbol
    tokens.push_back(new ASTNode("$"));
    for (const auto& token : tokens) {
        token->print();
    }
    return tokens;
}

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

};

#endif // PARSER_HEADER
