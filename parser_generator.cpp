#include <cassert>
#include <functional>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Struct to represent a Grammar Rule
struct Rule {
  string lhs;
  vector<string> rhs;

  Rule(const string &lhs, const vector<string> &rhs) : lhs(lhs), rhs(rhs) {}

  bool operator<(const Rule &other) const {
    return tie(lhs, rhs) < tie(other.lhs, other.rhs);
  }

  bool operator==(const Rule &other) const {
    return tie(lhs, rhs) == tie(other.lhs, other.rhs);
  }

  bool operator!=(const Rule &other) const { return !(*this == other); }
};

// Struct to represent an LR(1) Item
struct LR1Item {
  string lhs;
  vector<string> rhs;
  int dotPosition;
  string lookahead;

  LR1Item(const string &lhs, const vector<string> &rhs, int dotPosition,
          const string &lookahead)
      : lhs(lhs), rhs(rhs), dotPosition(dotPosition), lookahead(lookahead) {}

  bool operator<(const LR1Item &other) const {
    return tie(lhs, rhs, dotPosition, lookahead) <
           tie(other.lhs, other.rhs, other.dotPosition, other.lookahead);
  }

  bool operator==(const LR1Item &other) const {
    return tie(lhs, rhs, dotPosition, lookahead) ==
           tie(other.lhs, other.rhs, other.dotPosition, other.lookahead);
  }

  bool operator!=(const LR1Item &other) const { return !(*this == other); }

  void print() const {
    cout << lhs << " -> ";
    for (size_t i = 0; i < rhs.size(); ++i) {
      if (i == dotPosition) {
        cout << ". ";
      }
      cout << rhs[i] << " ";
    }
    if (dotPosition == rhs.size()) {
      cout << ". ";
    }
    cout << "| " << lookahead << endl;
  }
};

// Struct to represent an Action (Shift, Reduce, or Accept)
struct Action {
  enum ActionType { SHIFT, REDUCE, ACCEPT, NONE };
  ActionType actionType;
  int stateOrRule; // state for SHIFT, rule number for REDUCE, or -1 for ACCEPT

  Action(ActionType actionType = NONE, int stateOrRule = -1)
      : actionType(actionType), stateOrRule(stateOrRule) {}

  string toString() const {
    switch (actionType) {
    case SHIFT:
      return "Shift to state " + to_string(stateOrRule);
    case REDUCE:
      return "Reduce by rule " + to_string(stateOrRule);
    case ACCEPT:
      return "Accept";
    case NONE:
    default:
      return "None";
    }
  }
};

// Struct to represent a Goto transition
struct Goto {
  int state;

  Goto(int state = -1) : state(state) {}
};

// 2D vectors for the action and goto tables
vector<vector<Action>>
    actionTable; // state -> terminal symbol -> Action (Shift/Reduce/Accept)
vector<vector<Goto>> gotoTable; // state -> non-terminal -> new state

map<string, int> terminalToID;    // Mapping terminals to IDs
map<string, int> nonTerminalToID; // Mapping non-terminals to IDs
vector<string> terminals;         // List of terminal symbols
vector<string> nonTerminals;      // List of non-terminal symbols

vector<Rule> grammar; // The grammar rules

// Utility function to join a vector of strings into a single string
string join(const vector<string> &rhs) {
  stringstream ss;
  for (const string &symbol : rhs) {
    ss << symbol << " ";
  }
  return ss.str();
}

void printGrammar() {
  for (const auto &rule : grammar) {
    cout << rule.lhs << " -> " << join(rule.rhs) << endl;
  }
}

unordered_map<string, set<string>> firstSets; // First set for each symbol
unordered_map<string, set<string>>
    followSets; // Follow set for each non-terminal

// Function to compute the First set for a given symbol
set<string> computeFirst(const string &symbol) {
  set<string> result;
  if (terminalToID.find(symbol) != terminalToID.end()) {
    // If it's a terminal, its First set is just itself
    result.insert(symbol);
  } else if (nonTerminalToID.find(symbol) != nonTerminalToID.end()) {
    // If it's a non-terminal, compute its First set
    if (firstSets.find(symbol) == firstSets.end()) {
      // Compute First set for the non-terminal
      for (const auto &rule : grammar) {
        if (rule.lhs == symbol) {
          if (rule.rhs[0] == "ε") { // If ε is in the production
            result.insert("ε");
          } else {
            // Process the right-hand side
            for (const string &rhsSymbol : rule.rhs) {
              set<string> first = computeFirst(rhsSymbol);
              result.insert(first.begin(), first.end());
              if (first.find("ε") == first.end()) {
                break;
              }
            }
          }
        }
      }
      firstSets[symbol] = result;
    } else {
      result = firstSets[symbol];
    }
  }
  return result;
}

// Function to compute the Follow set for a non-terminal
void computeFollow(const string &nonTerminal) {
  if (followSets.find(nonTerminal) != followSets.end())
    return;

  // Initially, the Follow set of the start symbol contains END_OF_FILE
  if (nonTerminal == grammar[0].lhs) {
    followSets[nonTerminal].insert("END_OF_FILE");
  }

  // Iterate over all productions to compute the Follow set
  for (const auto &rule : grammar) {
    for (size_t i = 0; i < rule.rhs.size(); ++i) {
      if (rule.rhs[i] == nonTerminal) {
        // If a non-terminal appears in the RHS, add the First of the following
        // symbol to Follow
        if (i + 1 < rule.rhs.size()) {
          set<string> firstOfNext = computeFirst(rule.rhs[i + 1]);
          firstOfNext.erase("ε");
          followSets[nonTerminal].insert(firstOfNext.begin(),
                                         firstOfNext.end());
        }
        // If it's the last symbol or can derive ε, propagate the Follow of the
        // LHS
        if (i + 1 == rule.rhs.size() ||
            computeFirst(rule.rhs[i + 1]).count("ε")) {
          if (rule.lhs != nonTerminal) {
            computeFollow(rule.lhs);
            followSets[nonTerminal].insert(followSets[rule.lhs].begin(),
                                           followSets[rule.lhs].end());
          }
        }
      }
    }
  }
}

// Function to compute the closure of a set of LR(1) items
set<LR1Item> closure(const set<LR1Item> &items) {
  set<LR1Item> result = items;
  queue<LR1Item> toProcess;

  for (const auto &item : items) {
    toProcess.push(item);
  }

  while (!toProcess.empty()) {
    LR1Item currentItem = toProcess.front();
    toProcess.pop();

    if (currentItem.dotPosition < currentItem.rhs.size()) {
      string nextSymbol = currentItem.rhs[currentItem.dotPosition];

      // If it's a non-terminal, add all productions of that non-terminal
      if (nonTerminalToID.find(nextSymbol) !=
          nonTerminalToID.end()) { // Non-terminal
        for (const auto &rule : grammar) {
          if (rule.lhs == nextSymbol) {
            set<string> lookaheads = followSets[rule.lhs];
            for (const auto &lookahead : lookaheads) {
              LR1Item newItem = {rule.lhs, rule.rhs, 0, lookahead};
              if (result.insert(newItem).second) {
                toProcess.push(newItem);
              }
            }
          }
        }
      }
    }
  }

  return result;
}

// Function to compute the goto operation on a set of items by a symbol
set<LR1Item> gotoSet(const set<LR1Item> &items, const string &symbol) {
  set<LR1Item> result;
  for (const auto &item : items) {
    if (item.dotPosition < item.rhs.size() &&
        item.rhs[item.dotPosition] == symbol) {
      LR1Item newItem = item;
      newItem.dotPosition++;
      result.insert(newItem);
    }
  }
  return closure(result);
}

// Function to print the action and goto tables
void printParseTable() {
  // Print Action Table
  cout << "Action Table:\n";
  for (size_t state = 0; state < actionTable.size(); ++state) {
    for (size_t terminalID = 0; terminalID < terminals.size(); ++terminalID) {
      if (actionTable[state][terminalID].actionType != Action::NONE) {
        cout << "State " << state << ", Symbol " << terminals[terminalID]
             << ": " << actionTable[state][terminalID].toString() << "\n";
      }
    }
  }

  // Print Goto Table
  cout << "\nGoto Table:\n";
  for (size_t state = 0; state < gotoTable.size(); ++state) {
    for (size_t nonTerminalID = 0; nonTerminalID < nonTerminals.size();
         ++nonTerminalID) {
      if (gotoTable[state][nonTerminalID].state != -1) {
        cout << "State " << state << ", Non-terminal "
             << nonTerminals[nonTerminalID] << ": state "
             << gotoTable[state][nonTerminalID].state << "\n";
      }
    }
  }
}

vector<set<LR1Item>> states; // List of LR(1) states

// Function to generate the LR(1) parse table
void generateLR1ParseTable() {
  map<set<LR1Item>, int> stateToID; // Mapping from set of items to state ID

  LR1Item startItem(grammar[0].lhs, grammar[0].rhs, 0, "END_OF_FILE");
  set<LR1Item> initialState = closure({startItem});
  states.push_back(initialState);
  stateToID[initialState] = 0;

  queue<set<LR1Item>> stateQueue;
  stateQueue.push(initialState);

  int stateCounter = 1;
  while (!stateQueue.empty()) {
    set<LR1Item> currentState = stateQueue.front();
    stateQueue.pop();
    int currentStateID = stateToID[currentState];

    // Ensure actionTable and gotoTable are large enough
    while (actionTable.size() <= currentStateID) {
      actionTable.push_back(vector<Action>(terminals.size(), Action()));
    }
    while (gotoTable.size() <= currentStateID) {
      gotoTable.push_back(vector<Goto>(nonTerminals.size(), Goto()));
    }

    // Process all possible symbols (both terminals and non-terminals)
    set<string> symbols;
    for (const auto &item : currentState) {
      if (item.dotPosition < item.rhs.size()) {
        symbols.insert(item.rhs[item.dotPosition]);
      }
    }

    for (const string &symbol : symbols) {
      int symbolID = -1;
      if (terminalToID.find(symbol) != terminalToID.end()) {
        symbolID = terminalToID[symbol];
        set<LR1Item> nextState = gotoSet(currentState, symbol);
        if (stateToID.find(nextState) == stateToID.end()) {
          states.push_back(nextState);
          stateToID[nextState] = stateCounter++;
          stateQueue.push(nextState);
        }
        actionTable[currentStateID][symbolID] =
            Action(Action::SHIFT, stateToID[nextState]);
      } else if (nonTerminalToID.find(symbol) != nonTerminalToID.end()) {
        symbolID = nonTerminalToID[symbol];
        set<LR1Item> nextState = gotoSet(currentState, symbol);
        if (stateToID.find(nextState) == stateToID.end()) {
          states.push_back(nextState);
          stateToID[nextState] = stateCounter++;
          stateQueue.push(nextState);
        }
        gotoTable[currentStateID][symbolID] = Goto(stateToID[nextState]);
      }
    }

    // For each item in the state, handle reduction if dot is at the end
    for (const auto &item : currentState) {
      if (item.dotPosition == item.rhs.size()) {
        // If dot is at the end of the production, perform reduction
        if (item.lhs != grammar[0].lhs) { // Exclude start production
          int ruleId =
              distance(grammar.begin(), find_if(grammar.begin(), grammar.end(),
                                                [&item](const Rule &rule) {
                                                  return rule.lhs == item.lhs &&
                                                         rule.rhs == item.rhs;
                                                }));
          actionTable[currentStateID][terminalToID[item.lookahead]] =
              Action(Action::REDUCE, ruleId);
        }
        if (item.lhs == grammar[0].lhs && item.lookahead == "END_OF_FILE") {
          // Accept state for the start production
          actionTable[currentStateID][terminalToID["END_OF_FILE"]] =
              Action(Action::ACCEPT);
        }
      }
    }
  }

  printParseTable();
}

// camelCase to uppercase SNAKE_CASE
std::string toUpperSnakeCase(std::string input) {
  bool changed;
  do {
    changed = false;
    for (size_t i = 0; i < input.length() - 1; i++) {
      if (std::islower(input[i]) && std::isupper(input[i + 1])) {
        input.insert(i + 1, "_");
        changed = true;
        break;
      }
    }
  } while (changed);
  std::transform(input.begin(), input.end(), input.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return input;
}

void generateCSTHeaderFile() {
  ofstream headerFile("cst.h");
  if (!headerFile.is_open()) {

  }
  headerFile << "#ifndef CST_H\n";
  headerFile << "#define CST_H\n\n";
  headerFile << "#include <vector>\n";
  headerFile << "#include <string>\n\n";
  headerFile << "enum CSTNodeType {\n";
  for (const auto &nonTerminal : nonTerminals) {
    headerFile << "    " << toUpperSnakeCase(nonTerminal) << ",\n";
  }
  headerFile << "    TERMINAL,\n";
  headerFile << "};\n\n";
  headerFile << "std::string cstNodeTypeToString(CSTNodeType type) {\n";
  headerFile << "    switch (type) {\n";
  for (const auto &nonTerminal : nonTerminals) {
    headerFile << "        case " << toUpperSnakeCase(nonTerminal) << ":\n";
    headerFile << "            return \"" << toUpperSnakeCase(nonTerminal) << "\";\n";
  }
  headerFile << "        case TERMINAL:\n";
  headerFile << "            return \"TERMINAL\";\n";
  headerFile << "        default:\n";
  headerFile << "            return \"UNKNOWN\";\n";
  headerFile << "    }\n";
  headerFile << "}\n\n";
  headerFile << "class CSTNode {\n";
  headerFile << "public:\n";
  headerFile << "    CSTNodeType type;\n";
  headerFile << "    CSTNode *parent;\n";
  headerFile << "    std::vector<CSTNode *> children;\n";
  headerFile << "    CSTNode() {}\n";
  headerFile << "    CSTNode(CSTNodeType type) : type(type) {}\n";
  headerFile << "    void addChild(CSTNode *child) {\n";
  headerFile << "        child->parent = this;\n";
  headerFile << "        children.push_back(child);\n";
  headerFile << "    }\n\n";
  headerFile << "    virtual void print(int level = 0) const {\n";
  headerFile << "        for (int i = 0; i < level; ++i) std::cout << \"  \";  // Indentation for depth\n";
  headerFile << "        std::cout << cstNodeTypeToString(type) << std::endl;\n";
  headerFile << "        for (auto child : children) {\n";
  headerFile << "            child->print(level + 1);\n";
  headerFile << "        }\n";
  headerFile << "    }\n";
  headerFile << "};\n\n";
  headerFile << "enum CSTTerminalNodeType {\n";
  for (const auto &terminal : terminals) {
    headerFile << "    " << terminal << ",\n";
  }
  headerFile << "};\n\n";
  headerFile << "std::string cstTerminalNodeTypeToString(CSTTerminalNodeType type) {\n";
  headerFile << "    switch (type) {\n";
  for (const auto &terminal : terminals) {
    headerFile << "        case " << toUpperSnakeCase(terminal) << ":\n";
    headerFile << "            return \"" << toUpperSnakeCase(terminal) << "\";\n";
  }
  headerFile << "        default:\n";
  headerFile << "            return \"UNKNOWN\";\n";
  headerFile << "    }\n";
  headerFile << "}\n\n";
  headerFile << "class CSTTerminalNode : public CSTNode {\n";
  headerFile << "public:\n";
  headerFile << "    CSTTerminalNodeType type;\n";
  headerFile << "    std::string value;\n";
  headerFile << "    CSTTerminalNode(CSTTerminalNodeType type, const std::string &value) : CSTNode(CSTNodeType::TERMINAL), type(type), value(value) {}\n";
  headerFile << "    CSTTerminalNode(CSTTerminalNodeType type) : CSTTerminalNode(type, \"\") {}\n";
  headerFile << "    void print(int level = 0) const override  {\n";
  headerFile << "        for (int i = 0; i < level; ++i) std::cout << \"  \";  // Indentation for depth\n";
  headerFile << "        std::cout << cstTerminalNodeTypeToString(type);\n";
  headerFile << "        if (!value.empty()) {\n";
  headerFile << "            std::cout << \": \" << value;\n";
  headerFile << "        }\n";
  headerFile << "        std::cout << std::endl;\n";
  headerFile << "    }\n";
  headerFile << "};\n\n";
  headerFile << "#endif";
}

// Function to generate the header file
void generateParserHeaderFile() {
  ofstream headerFile("parser.h");

  // Write the header guards
  headerFile << "#ifndef PARSER_H\n";
  headerFile << "#define PARSER_H\n\n";

  // Write the includes
  headerFile << "#include <algorithm>\n";
  headerFile << "#include <string>\n";
  headerFile << "#include <map>\n";
  headerFile << "#include <vector>\n\n";

  // Write the Action struct definition
  headerFile << "struct Action {\n";
  headerFile << "    enum ActionType {\n";
  headerFile << "        SHIFT,\n";
  headerFile << "        REDUCE,\n";
  headerFile << "        ACCEPT,\n";
  headerFile << "        NONE\n";
  headerFile << "    };\n";
  headerFile << "    ActionType actionType;\n";
  headerFile
      << "    int stateOrRule; // For SHIFT: state; For REDUCE: rule index\n";
  headerFile << "};\n\n";

  // Write the Goto struct definition
  headerFile << "struct Goto {\n";
  headerFile << "    int state;  // The state to go to for a non-terminal\n";
  headerFile << "};\n\n";

  // Write the Rule struct definition
  headerFile << "struct Rule {\n";
  headerFile << "    std::string lhs;  // Left-hand side of the rule\n";
  headerFile << "    std::vector<std::string> rhs;  // Right-hand side of the "
                "rule (sequence of symbols)\n";
  headerFile << "};\n\n";

  // Write terminal and non-terminal mappings
  headerFile << "static const int NUM_TERMINALS = " << terminals.size()
             << ";\n";
  headerFile << "static const int NUM_NON_TERMINALS = " << nonTerminals.size()
             << ";\n";
  headerFile << "static const int NUM_STATES = " << actionTable.size()
             << ";\n\n";

  // Terminal and non-terminal mappings
  headerFile << "static const std::map<std::string, int> terminalToID = {\n";
  for (const auto &entry : terminalToID) {
    headerFile << "    {\"" << entry.first << "\", " << entry.second << "},\n";
  }
  headerFile << "};\n\n";

  headerFile << "static const std::map<std::string, int> nonTerminalToID = {\n";
  for (const auto &entry : nonTerminalToID) {
    headerFile << "    {\"" << entry.first << "\", " << entry.second << "},\n";
  }
  headerFile << "};\n\n";

  // Write the action table
  headerFile
      << "static const std::vector<std::vector<Action>> actionTable = {\n";
  for (size_t i = 0; i < actionTable.size(); ++i) {
    headerFile << "    { ";
    for (size_t j = 0; j < actionTable[i].size(); ++j) {
      headerFile << "{Action::"
                 << (actionTable[i][j].actionType == Action::SHIFT    ? "SHIFT"
                     : actionTable[i][j].actionType == Action::REDUCE ? "REDUCE"
                     : actionTable[i][j].actionType == Action::ACCEPT ? "ACCEPT"
                                                                      : "NONE")
                 << ", " << actionTable[i][j].stateOrRule << "}, ";
    }
    headerFile << "},\n";
  }
  headerFile << "};\n\n";

  // Write the goto table
  headerFile << "static const std::vector<std::vector<Goto>> gotoTable = {\n";
  for (size_t i = 0; i < gotoTable.size(); ++i) {
    headerFile << "    { ";
    for (size_t j = 0; j < gotoTable[i].size(); ++j) {
      headerFile << "{" << gotoTable[i][j].state << "}, ";
    }
    headerFile << "},\n";
  }
  headerFile << "};\n\n";

  // Write the number of symbols per rule
  headerFile << "static const std::map<int, int> ruleSymbolCount = {\n";
  for (size_t i = 0; i < grammar.size(); ++i) {
    headerFile << "    {" << i << ", " << grammar[i].rhs.size() << "},\n";
  }
  headerFile << "};\n\n";

  // Write the grammar (rules)
  headerFile << "static const std::vector<Rule> grammar = {\n";
  for (size_t i = 0; i < grammar.size(); ++i) {
    headerFile << "    {\"" << grammar[i].lhs << "\", {";
    for (size_t j = 0; j < grammar[i].rhs.size(); ++j) {
      headerFile << "\"" << grammar[i].rhs[j] << "\"";
      if (j < grammar[i].rhs.size() - 1) {
        headerFile << ", ";
      }
    }
    headerFile << "}},\n";
  }
  headerFile << "};\n\n";

  // Close the header guard
  headerFile << "#endif // PARSER_H\n";

  // Close the file
  headerFile.close();
}

#include "grammar_parser.h"

void traversePostOrder(GrammarParser::CSTNode* node, std::function<void(GrammarParser::CSTNode*)> callback) {
    for (GrammarParser::CSTNode* child : node->children) {
        traversePostOrder(child, callback);
    }
    callback(node);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
      cerr << "Usage: " << argv[0] << " <input_file>" << endl;
      return 1;
  }

  string inputString = GrammarParser::readFile(argv[1]);
  vector<GrammarParser::CSTNode *> input = GrammarParser::tokenize(inputString);

  try {
      GrammarParser::CSTNode* cstRoot = GrammarParser::parse(input);  // Start parsing and generate the CST
      if (cstRoot) {
          vector<vector<string>> rhsOptions;
          vector<string> rhs;
          traversePostOrder(cstRoot, [&](GrammarParser::CSTNode* node) {
            if (node->type == GrammarParser::CSTNodeType::RULE && node->children.size() == 4) {
              string lhs = dynamic_cast<GrammarParser::CSTTerminalNode *>(node->children[0])->value;
              if (rhsOptions.empty()) {
                throw runtime_error("Empty rule");
              }
              for (vector<string> rhs : rhsOptions) {
                grammar.push_back(Rule(lhs, rhs));
              }
              rhsOptions.clear();
            }
            if (node->type == GrammarParser::CSTNodeType::OPTION && node->children.size() == 1) {
              if (rhsOptions.empty()) {
                rhs.erase(rhs.begin());
              }
              if (rhs.empty()) {
                throw runtime_error("Empty option");
              }
              rhsOptions.push_back(rhs);
              rhs.clear();
            }
            if (node->type == GrammarParser::CSTNodeType::TERMINAL) {
              GrammarParser::CSTTerminalNode *terminal = dynamic_cast<GrammarParser::CSTTerminalNode *>(node);
              GrammarParser::CSTTerminalNodeType type = terminal->type;
              if (type == GrammarParser::CSTTerminalNodeType::IDENTIFIER) {
                rhs.push_back(terminal->value);
              }
            }
          });
      } else {
          cout << "No CST generated." << endl;
      }
  } catch (const runtime_error& e) {
      cerr << "Error: " << e.what() << endl;
      return 1;
  }

  // Print the grammar
  for (const auto &rule : grammar) {
    cout << rule.lhs << " -> ";
    for (const auto &symbol : rule.rhs) {
      cout << symbol << " ";
    }
    cout << endl;
  }

  // Populate the terminal and non-terminal symbols

  for (const auto &rule : grammar) {
    if (find(nonTerminals.begin(), nonTerminals.end(), rule.lhs) == nonTerminals.end()) {
      nonTerminals.push_back(rule.lhs);
    }
  }

  for (const auto &rule : grammar) {
    for (const auto &symbol : rule.rhs) {
      if (find(nonTerminals.begin(), nonTerminals.end(), symbol) == nonTerminals.end() &&
          find(terminals.begin(), terminals.end(), symbol) == terminals.end()) {
        terminals.push_back(symbol);
      }
    }
  }

  terminals.push_back("END_OF_FILE");

  for (int i = 0; i < terminals.size(); ++i) {
    terminalToID[terminals[i]] = i;
  }

  for (int i = 0; i < nonTerminals.size(); ++i) {
    nonTerminalToID[nonTerminals[i]] = i;
  }

  // Compute follow for each non-terminal
  for (const auto &nonTerminal : nonTerminals) {
    computeFollow(nonTerminal);
  }

  generateLR1ParseTable();
  generateParserHeaderFile();
  generateCSTHeaderFile();

  return 0;
}
