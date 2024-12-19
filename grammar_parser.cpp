#include "grammar_parser.h"

#include <stack>
#include <sstream>
#include <fstream>

using namespace std;
using namespace GrammarParser;

// The LR(1) parser function
CSTNode* GrammarParser::parse(const vector<CSTNode *>& input) {
    stack<int> stateStack;  // Stack to store states
    stack<CSTNode*> astStack;  // Stack to store AST nodes for each symbol
    
    stateStack.push(0);  // Initial state is 0
    
    size_t inputIndex = 0;  // Pointer to the current symbol in the input
    CSTNode *currentSymbol = input[inputIndex];  // Current symbol to process
    
    while (true) {
        int currentState = stateStack.top();  // Top of the state stack
        CSTTerminalNodeType type = dynamic_cast<CSTTerminalNode*>(currentSymbol)->type;
        cout << "Current State: " << currentState << ", Current Symbol: " << cstTerminalNodeTypeToString(type) << endl;

        // Get the action for the current state and symbol
        Action currentAction = actionTable[currentState][type];

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
                    currentSymbol = new CSTTerminalNode(CSTTerminalNodeType::END_OF_FILE);
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
                vector<CSTNode*> rhsNodes;
                for (size_t i = 0; i < rule.rhs.size(); ++i) {
                    stateStack.pop();
                    CSTNode* rhsNode = astStack.top();
                    astStack.pop();
                    rhsNodes.push_back(rhsNode);  // Collect the AST nodes for the RHS symbols
                }

                // Create a new AST node for the left-hand side (LHS) of the rule
                CSTNode* parentNode = new CSTNode((CSTNodeType)nonTerminalToID.at(rule.lhs));

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

// Tokenizer function that returns CSTNode instances for recognized tokens
vector<CSTNode*> GrammarParser::tokenize(const string& input) {
    vector<CSTNode*> tokens;
    int index = 0;

    while (index < input.length()) {
        if (isspace(input[index])) {
            index++;  // Skip whitespace
            continue;
        }
        static const map<char, CSTTerminalNodeType> punctuatorMap = {
            {':', CSTTerminalNodeType::COLON},
            {';', CSTTerminalNodeType::SEMICOLON},
            {'|', CSTTerminalNodeType::VERTICAL_BAR}
        };

        auto punctuatorIter = punctuatorMap.find(input[index]);
        if (punctuatorIter != punctuatorMap.end()) {
            tokens.push_back(new CSTTerminalNode(punctuatorIter->second));
            index++;
            continue;
        }

        // Handle IDENTIFIER
        if (isalpha(input[index]) || input[index] == '_') {
            string identifier;
            while (index < input.length() && (isalnum(input[index]) || input[index] == '_')) {
                identifier += input[index++];
            }
            CSTNode* identifierNode = new CSTTerminalNode(CSTTerminalNodeType::IDENTIFIER, identifier);
            tokens.push_back(identifierNode);
            continue;
        }

        // Handle unrecognized characters (optional: throw error)
        cerr << "Unrecognized character: " << input[index] << endl;
        index++;
    }

    // Add end of input symbol
    tokens.push_back(new CSTTerminalNode(CSTTerminalNodeType::END_OF_FILE));
    
    // For debugging: print tokens
    for (const auto& token : tokens) {
        token->print();
    }

    return tokens;
}

string GrammarParser::readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
