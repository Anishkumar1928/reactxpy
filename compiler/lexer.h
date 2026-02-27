#pragma once
#include <string>
#include <vector>
#include <cstddef>

// ============================================
// Token Types
// ============================================
enum TokenType {

    // keywords
    DEF,
    RETURN,
    IMPORT,
    FROM,
    // Python Scope Control
    INDENT,
    DEDENT,

    IDENTIFIER,

    // ⭐ JS literals
    TRUE,
    FALSE,
    NULLTOK,

    // JSX
    TAG_OPEN,
    TAG_CLOSE,
    SELF_CLOSE,

    ATTRIBUTE_NAME,
    ATTRIBUTE_VALUE,
    ATTRIBUTE_EXPR,
    EQUAL,

    EXPRESSION,
    TEXT,

    // syntax
    LPAREN,
    RPAREN,
    COLON,
    COMMA,
    LBRACKET,
    RBRACKET,
    STAR,

    STRING,

    END
};

// ============================================

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

#include "errors.h"

class Lexer {
private:
    std::string source;
    std::size_t pos;
    bool insideJSX;
    int jsxDepth;
    int openBrackets;
    
    // Position tracking for errors
    int currentLine;
    int currentColumn;
    ErrorReporter& reporter;
    
    // PEP-8 Whitespace Indentation Tracking Overlay
    std::vector<int> indentStack;

public:
    explicit Lexer(const std::string& src, ErrorReporter& rep);
    std::vector<Token> tokenize();
};