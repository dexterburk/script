#ifndef CST_H
#define CST_H

#include <vector>

enum CSTNodeType {
    PROGRAM,
    FUNCTION_LIST,
    FUNCTION,
    TYPE,
    PARAMETER_LIST,
    PARAMETER,
    STATEMENT_LIST,
    STATEMENT,
    EXPRESSION,
    TERM,
    FACTOR,
    TERMINAL,
};

std::string cstNodeTypeToString(CSTNodeType type) {
    switch (type) {
        case PROGRAM:
            return "PROGRAM";
        case FUNCTION_LIST:
            return "FUNCTION_LIST";
        case FUNCTION:
            return "FUNCTION";
        case TYPE:
            return "TYPE";
        case PARAMETER_LIST:
            return "PARAMETER_LIST";
        case PARAMETER:
            return "PARAMETER";
        case STATEMENT_LIST:
            return "STATEMENT_LIST";
        case STATEMENT:
            return "STATEMENT";
        case EXPRESSION:
            return "EXPRESSION";
        case TERM:
            return "TERM";
        case FACTOR:
            return "FACTOR";
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
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACE,
    RIGHT_BRACE,
    INT,
    COMMA,
    RETURN,
    SEMICOLON,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    NUMBER,
    END_OF_FILE,
};

std::string cstTerminalNodeTypeToString(CSTTerminalNodeType type) {
    switch (type) {
        case IDENTIFIER:
            return "IDENTIFIER";
        case LEFT_PARENTHESIS:
            return "LEFT_PARENTHESIS";
        case RIGHT_PARENTHESIS:
            return "RIGHT_PARENTHESIS";
        case LEFT_BRACE:
            return "LEFT_BRACE";
        case RIGHT_BRACE:
            return "RIGHT_BRACE";
        case INT:
            return "INT";
        case COMMA:
            return "COMMA";
        case RETURN:
            return "RETURN";
        case SEMICOLON:
            return "SEMICOLON";
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case ASTERISK:
            return "ASTERISK";
        case SLASH:
            return "SLASH";
        case NUMBER:
            return "NUMBER";
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