#include "parser.h"
#include <stdexcept>

using namespace std;

// --------------------------------
// Constructor
// --------------------------------
Parser::Parser(const vector<Token>& t, ErrorReporter& rep) : reporter(rep) {
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
// HELPER: Check if token is an assignment operator
// =================================
bool isAssignmentOperator(const std::string& val) {
    return val == "=" || val == "+=" || val == "-=" || val == "*=" || val == "/=" || val == "%=";
}

// =================================
// HELPER: Validate expression for empty lambda
// =================================
bool hasEmptyLambda(const std::string& expr) {
    // Check for "lambda:" or "lambda :" with nothing meaningful after
    size_t lambdaPos = expr.find("lambda");
    if (lambdaPos == string::npos) return false;
    
    size_t colonPos = expr.find(':', lambdaPos);
    if (colonPos == string::npos) return false;
    
    // Check if there's anything substantial after the colon
    string afterColon = expr.substr(colonPos + 1);
    // Trim whitespace
    size_t start = afterColon.find_first_not_of(" \t\n\r");
    if (start == string::npos) return true; // Nothing but whitespace
    
    // Check if next non-whitespace char is a closing paren or bracket (empty)
    char nextChar = afterColon[start];
    return nextChar == ')' || nextChar == ']' || nextChar == ',' || nextChar == '\0';
}

// =================================
// IMPORT PARSER
// =================================
ImportNode Parser::parseImport() {

    ImportNode imp;
    imp.type = IMPORT_DEFAULT;

    match(IMPORT);

    if (!check(IDENTIFIER)) {
        Token errToken = tokens[pos - 1];
        reporter.report("RX200", "SyntaxError", "Expected identifier after import", errToken.line, errToken.column, errToken.value.length());
        return imp;
    }

    imp.module = advance().value;

    if (match(FROM)) {
        if (check(STRING))
            advance();
    }

    return imp;
}

// =================================
// PROGRAM PARSER
// =================================
ProgramNode Parser::parseProgram() {

    ProgramNode program;

    while (!isAtEnd()) {

        if (check(IMPORT))
            program.imports.push_back(parseImport());

        else if (check(DEF))
            program.functions.push_back(parseFunction());

        else
            advance();
    }

    return program;
}

// =================================
// FUNCTION PARSER  ⭐ FIXED
// Proper Python INDENT → JS blocks
// =================================
FunctionNode Parser::parseFunction() {

    FunctionNode fn;

    match(DEF);

    if (!check(IDENTIFIER)) {
        Token errToken = tokens[pos - 1];
        reporter.report("RX201", "SyntaxError", "Expected function name", errToken.line, errToken.column, errToken.value.length());
        return fn;
    }

    fn.name = advance().value;

    // -------- parameters --------
    match(LPAREN);

    while (!check(RPAREN) && !isAtEnd()) {

        if (check(IDENTIFIER))
            fn.params.push_back(advance().value);

        if (check(COMMA))
            advance();
    }

    match(RPAREN);
    match(COLON);

    string body;
    int indentLevel = 0;

    // =============================
    // BODY COLLECTION (REAL FIX)
    // =============================
    while (!isAtEnd()) {

        // next function begins
        if (check(DEF) && indentLevel == 0)
            break;

        // ----- INDENT -----
        if (match(INDENT)) {
            body += "{\n";
            indentLevel++;
            continue;
        }

        // ----- DEDENT -----
        if (match(DEDENT)) {
            body += "}\n";
            indentLevel--;
            continue;
        }

        // ----- RETURN -----
        if (match(RETURN)) {
            body += "return ";
            continue;
        }

        // ----- INLINE JSX -----
        if (check(TAG_OPEN)) {
            fn.jsx_list.push_back(parseJSX());
            body += "__JSX" + to_string(fn.jsx_list.size() - 1) + "__ ";
            continue;
        }

        if (check(TAG_CLOSE)) {
            Token errToken = advance();
            reporter.report("RX202", "SyntaxError", "Unexpected closing tag </" + errToken.value + "> without opening tag", errToken.line, errToken.column, errToken.value.length() + 3);
            continue;
        }

        Token t = advance();

        // ----- VALIDATION: Incomplete assignment (e.g., "a, b =" or "x =") -----
        if (t.value == "=") {
            // Check if next token indicates missing RHS
            // RHS could be missing if followed by: return, dedent, end, def, or another statement
            bool missingRHS = check(DEDENT) || check(END) || check(DEF) || check(RETURN) ||
                              (current().type == IDENTIFIER && isAssignmentOperator(current().value));
            
            // Also check if we're at a statement boundary (next token starts a new statement)
            if (!missingRHS && current().type == IDENTIFIER) {
                string nextVal = current().value;
                // Common statement-starting keywords that shouldn't follow '='
                if (nextVal == "return" || nextVal == "if" || nextVal == "for" || 
                    nextVal == "while" || nextVal == "def" || nextVal == "import" ||
                    nextVal == "pass" || nextVal == "break" || nextVal == "continue") {
                    missingRHS = true;
                }
            }
            
            if (missingRHS) {
                reporter.report("RX203", "SyntaxError", "Incomplete assignment - missing right-hand side expression", t.line, t.column, 1);
            }
        }

        // ----- VALIDATION: Incomplete if statement -----
        if (t.value == "if" || t.value == "elif") {
            // Check if 'if' is followed immediately by colon, dedent/end, or indent (newline after if)
            if (check(COLON) || check(DEDENT) || check(END) || check(DEF) || check(INDENT) || check(RETURN)) {
                reporter.report("RX208", "SyntaxError", "Incomplete 'if' statement - missing condition", t.line, t.column, t.value.length());
            }
        }

        // ----- VALIDATION: Empty lambda in expressions -----
        if (t.type == EXPRESSION || t.type == ATTRIBUTE_EXPR) {
            if (hasEmptyLambda(t.value)) {
                reporter.report("RX209", "SyntaxError", "Empty lambda expression - lambda requires a body after the colon", t.line, t.column, t.value.length());
            }
        }
        
        // ----- VALIDATION: Empty lambda at token level (e.g., useEffect(lambda:)) -----
        if (t.value == "lambda") {
            // Check if next token is ':' followed by ')', ']', ',', or dedent
            if (check(COLON)) {
                // Look ahead to see what follows the colon
                size_t savedPos = pos;
                advance(); // consume ':'
                bool isEmpty = isAtEnd() || check(RPAREN) || check(RBRACKET) || check(COMMA) || 
                               check(DEDENT) || check(END) || check(RPAREN) ||
                               (check(IDENTIFIER) && (current().value == ")" || current().value == "]"));
                pos = savedPos; // restore position
                
                if (isEmpty) {
                    reporter.report("RX209", "SyntaxError", "Empty lambda expression - lambda requires a body after the colon", t.line, t.column, 6);
                }
            }
        }

        // preserve strings safely
        if (t.type == STRING) {

            string escaped;
            for(char c : t.value){
                if(c=='"') escaped+="\\\"";
                else if(c=='\\') escaped+="\\\\";
                else if(c=='\n') escaped+="\\n";
                else escaped+=c;
            }

            body += "\"" + escaped + "\"";
        }
        else {
            body += t.value;
        }

        body += " ";
    }

    fn.body = body;
    return fn;
}

// =================================
// JSX PARSER
// =================================
shared_ptr<JSXNode> Parser::parseJSX() {

    if (!match(TAG_OPEN)) {
        Token errToken = current();
        reporter.report("RX207", "SyntaxError", "Expected opening tag", errToken.line, errToken.column, errToken.value.length() + 2); // +2 for < and > if available
        return make_shared<JSXNode>();
    }

    auto node = make_shared<JSXNode>();
    node->tag = tokens[pos - 1].value;

    // ---------- props ----------
    while (check(ATTRIBUTE_NAME)) {

        string key = advance().value;
        bool hasEqual = match(EQUAL);

        PropValue prop;

        if (check(ATTRIBUTE_VALUE)) {
            prop.value = advance().value;
        }
        else if (check(ATTRIBUTE_EXPR) || check(EXPRESSION)) {
            prop.isExpression = true;
            prop.value = advance().value;
            
            // ----- VALIDATION: Empty lambda in attribute expressions -----
            if (hasEmptyLambda(prop.value)) {
                reporter.report("RX209", "SyntaxError", "Empty lambda expression in attribute - lambda requires a body after the colon", current().line, current().column, prop.value.length());
            }
        }
        else if (hasEqual) {
            // Attribute has '=' but no value followed
            reporter.report("RX206", "SyntaxError", "Empty attribute value - '" + key + "=' requires a value", current().line, current().column, 1);
        }

        node->props[key] = prop;
    }

    // self closing
    if (match(SELF_CLOSE)) {
        node->selfClosing = true;
        return node;
    }

    // ---------- children ----------
    while (!isAtEnd()) {

        if (check(TEXT)) {
            JSXChild c;
            c.type = CHILD_TEXT;
            c.value = advance().value;
            node->children.push_back(c);
        }
        else if (check(EXPRESSION)) {
            JSXChild c;
            c.type = CHILD_EXPR;
            c.value = advance().value;
            node->children.push_back(c);
        }
        else if (check(TAG_OPEN)) {
            JSXChild c;
            c.type = CHILD_ELEMENT;
            c.element = parseJSX();
            node->children.push_back(c);
        }
        else if (check(TAG_CLOSE)) {
            Token closeToken = advance();
            if (closeToken.value != node->tag) {
                reporter.report("RX204", "SyntaxError", "Expected closing tag </" + node->tag + "> but found </" + closeToken.value + ">", closeToken.line, closeToken.column, closeToken.value.length() + 3);
            }
            return node;
        }
        else {
            advance();
        }
    }

    Token errToken = current();
    if (!reporter.hasErrors()) {
        reporter.report("RX205", "SyntaxError", "JSX parse failed/unterminated tag", errToken.line, errToken.column, errToken.value.length());
    }
    return node;
}