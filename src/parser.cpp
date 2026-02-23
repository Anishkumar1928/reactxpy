#include "parser.h"
#include <stdexcept>

using namespace std;

// --------------------------------
Parser::Parser(const vector<Token>& t) {
    tokens = t;
    pos = 0;
}

bool Parser::isAtEnd() {
    return pos >= tokens.size() ||
           tokens[pos].type == END;
}

Token Parser::current() {
    if (isAtEnd()) return tokens.back();
    return tokens[pos];
}

Token Parser::advance() {
    if (!isAtEnd()) pos++;
    return tokens[pos - 1];
}

bool Parser::match(TokenType type) {
    if (!isAtEnd() && current().type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return current().type == type;
}

// =================================
// PROGRAM
// =================================
ProgramNode Parser::parseProgram() {

    ProgramNode program;

    while (!isAtEnd()) {
        if (check(DEF))
            program.functions.push_back(parseFunction());
        else
            advance();
    }

    return program;
}

// =================================
// ⭐ FUNCTION PARSER (FINAL)
// =================================
FunctionNode Parser::parseFunction() {

    FunctionNode fn;

    match(DEF);

    if (!check(IDENTIFIER))
        throw runtime_error("Expected function name");

    fn.name = advance().value;

    // ---------- PARAMETERS ----------
    match(LPAREN);

    while (!check(RPAREN) && !isAtEnd()) {

        if (check(IDENTIFIER))
            fn.params.push_back(advance().value);

        if (check(COMMA))
            advance();
    }

    match(RPAREN);
    match(COLON);

    // ---------- COMPONENT ----------
    if (match(RETURN)) {
        fn.jsx = parseJSX();
        return fn;
    }

    // ---------- EVENT FUNCTION ----------
    string body;

    while (!isAtEnd() && !check(DEF)) {

        Token t = advance();

        if (t.type == STRING)
            body += "\"" + t.value + "\"";
        else
            body += t.value;

        if (t.type != LPAREN &&
            t.type != RPAREN &&
            t.type != COMMA)
            body += " ";
    }

    fn.body = body;
    fn.jsx = nullptr;

    return fn;
}

// =================================
// JSX PARSER
// =================================
shared_ptr<JSXNode> Parser::parseJSX() {

    if (!match(TAG_OPEN))
        throw runtime_error("Expected opening tag");

    auto node = make_shared<JSXNode>();
    node->tag = tokens[pos - 1].value;

    // props
    while (check(ATTRIBUTE_NAME)) {

        string key = advance().value;
        match(EQUAL);

        PropValue prop;

        if (check(ATTRIBUTE_VALUE))
            prop.value = advance().value;

        else if (check(ATTRIBUTE_EXPR) || check(EXPRESSION)) {
            prop.isExpression = true;
            prop.value = advance().value;
        }

        node->props[key] = prop;
    }

    if (match(SELF_CLOSE)) {
        node->selfClosing = true;
        return node;
    }

    while (!isAtEnd()) {

        if (check(TEXT)) {
            JSXChild c{CHILD_TEXT, advance().value};
            node->children.push_back(c);
        }
        else if (check(EXPRESSION)) {
            JSXChild c{CHILD_EXPR, advance().value};
            node->children.push_back(c);
        }
        else if (check(TAG_OPEN)) {
            JSXChild c;
            c.type = CHILD_ELEMENT;
            c.element = parseJSX();
            node->children.push_back(c);
        }
        else if (check(TAG_CLOSE)) {
            advance();
            return node;
        }
        else {
            advance();
        }
    }

    throw runtime_error("JSX parse failed");
}