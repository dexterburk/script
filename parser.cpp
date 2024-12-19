#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <sstream>
#include <stdexcept>
#include "parser.h"  // Include the generated header file

using namespace std;

struct ASTNode {
    string symbol;  // The symbol of this node (terminal or non-terminal)
    string value;  // The value of this node
    vector<ASTNode*> children;  // The children of this node (for non-terminals)
    
    ASTNode(const string& symbol) : symbol(symbol) {}
    ASTNode(const string& symbol, const string& value) : symbol(symbol), value(value) {}

    // Function to add a child node
    void addChild(ASTNode* child) {
        children.push_back(child);
    }

    // Function to print the tree (for debugging purposes)
    void print(int level = 0) const {
        for (int i = 0; i < level; ++i) cout << "  ";  // Indentation for depth
        cout << symbol;
        if (!value.empty()) {
            cout << " (" << value << ")";
        }
        cout << endl;
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

                // Create a new AST node for the left-hand side (LHS) of the rule
                ASTNode* parentNode = new ASTNode(rule.lhs);

                // Reverse the order of the RHS nodes
                reverse(rhsNodes.begin(), rhsNodes.end());

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

// Tokenizer function that returns ASTNode instances for recognized tokens
vector<ASTNode*> tokenize(const string& input) {
    vector<ASTNode*> tokens;
    int index = 0;

    while (index < input.length()) {
        if (isspace(input[index])) {
            index++;  // Skip whitespace
            continue;
        }
        static const map<char, string> operatorMap = {
            {',', "COMMA"},
            {';', "SEMICOLON"},
            {'{', "LEFT_BRACE"},
            {'}', "RIGHT_BRACE"},
            {'(', "LEFT_PARENTHESIS"},
            {')', "RIGHT_PARENTHESIS"},
            {'+', "PLUS"},
            {'-', "MINUS"},
            {'*', "ASTERISK"},
            {'/', "SLASH"},
        };

        auto operatorIter = operatorMap.find(input[index]);
        if (operatorIter != operatorMap.end()) {
            tokens.push_back(new ASTNode(operatorIter->second));
            index++;
            continue;
        }

        static const map<string, string> keywordMap = {
            {"return", "RETURN"},
            {"int", "INT"},
        };

        for (const auto& keywordPair : keywordMap) {
            if (input.substr(index, keywordPair.first.length()) == keywordPair.first) {
                tokens.push_back(new ASTNode(keywordPair.second));
                index += keywordPair.first.length();
                break;
            }
        }

        // Handle IDENTIFIER
        if (isalpha(input[index]) || input[index] == '_') {
            string identifier;
            while (index < input.length() && (isalnum(input[index]) || input[index] == '_')) {
                identifier += input[index++];
            }
            ASTNode* identifierNode = new ASTNode("IDENTIFIER", identifier);
            tokens.push_back(identifierNode);
            continue;
        }

        // Handle NUMBER
        if (isdigit(input[index])) {
            string number;
            while (index < input.length() && isdigit(input[index])) {
                number += input[index++];
            }
            ASTNode* numberNode = new ASTNode("NUMBER", number);
            tokens.push_back(numberNode);
            continue;
        }

        // Handle unrecognized characters (optional: throw error)
        cerr << "Unrecognized character: " << input[index] << endl;
        index++;
    }

    // Add end of input symbol
    tokens.push_back(new ASTNode("$"));
    
    // For debugging: print tokens
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string inputString = readFile(argv[1]);
    vector<ASTNode *> input = tokenize(inputString);

    try {
        ASTNode* astRoot = parse(input);  // Start parsing and generate the AST
        if (astRoot) {
            cout << "AST for the input:" << endl;
            astRoot->print();  // Print the AST
        } else {
            cout << "No AST generated." << endl;
        }
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
