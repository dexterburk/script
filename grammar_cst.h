#ifndef CST_H
#define CST_H

#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <iostream>

enum CSTNodeType {
    GRAMMAR,
    RULE_LIST,
    RULE,
    OPTION_LIST,
    OPTION,
    IDENTIFIER_LIST,
    TERMINAL,
};

inline std::string cstNodeTypeToString(CSTNodeType type) {
    switch (type) {
        case GRAMMAR:
            return "GRAMMAR";
        case RULE_LIST:
            return "RULE_LIST";
        case RULE:
            return "RULE";
        case OPTION_LIST:
            return "OPTION_LIST";
        case OPTION:
            return "OPTION";
        case IDENTIFIER_LIST:
            return "IDENTIFIER_LIST";
        case TERMINAL:
            return "TERMINAL";
        default:
            return "UNKNOWN";
    }
}

class CSTNode {
public:
    CSTNodeType type;
    CSTNode *parent;
    std::vector<CSTNode *> children;
    CSTNode() {}
    CSTNode(CSTNodeType type) : type(type) {}
    void addChild(CSTNode *child) {
        child->parent = this;
        children.push_back(child);
    }

    virtual void print(int level = 0) const {
        for (int i = 0; i < level; ++i) std::cout << "  ";  // Indentation for depth
        std::cout << cstNodeTypeToString(type) << std::endl;
        for (auto child : children) {
            child->print(level + 1);
        }
    }
};

enum CSTTerminalNodeType {
    IDENTIFIER,
    COLON,
    SEMICOLON,
    VERTICAL_BAR,
    END_OF_FILE,
};

inline std::string cstTerminalNodeTypeToString(CSTTerminalNodeType type) {
    switch (type) {
        case IDENTIFIER:
            return "IDENTIFIER";
        case COLON:
            return "COLON";
        case SEMICOLON:
            return "SEMICOLON";
        case VERTICAL_BAR:
            return "VERTICAL_BAR";
        case END_OF_FILE:
            return "END_OF_FILE";
        default:
            return "UNKNOWN";
    }
}

class CSTTerminalNode : public CSTNode {
public:
    CSTTerminalNodeType type;
    std::string value;
    CSTTerminalNode(CSTTerminalNodeType type, const std::string &value) : CSTNode(CSTNodeType::TERMINAL), type(type), value(value) {}
    CSTTerminalNode(CSTTerminalNodeType type) : CSTTerminalNode(type, "") {}
    void print(int level = 0) const override  {
        for (int i = 0; i < level; ++i) std::cout << "  ";  // Indentation for depth
        std::cout << cstTerminalNodeTypeToString(type);
        if (!value.empty()) {
            std::cout << ": " << value;
        }
        std::cout << std::endl;
    }
};

#endif