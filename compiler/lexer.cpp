#include "lexer.h"
#include <cctype>

using namespace std;

inline bool isAlpha(char c){ return std::isalpha((unsigned char)c); }
inline bool isAlnum(char c){ return std::isalnum((unsigned char)c); }
inline bool isSpace(char c){ return std::isspace((unsigned char)c); }

// --------------------------------
string normalizeJSXText(const string& input){

    string result;
    bool last=false;

    for(char c:input){

        if(c=='\n'||c=='\r'||c=='\t')
            c=' ';

        if(isSpace(c)){
            if(!last){
                result+=' ';
                last=true;
            }
        }else{
            result+=c;
            last=false;
        }
    }

    if(!result.empty() && result.front()==' ')
        result.erase(result.begin());

    return result;
}

// --------------------------------
string readExpression(const string& src,size_t& pos){

    string expr;
    int depth=1;

    while(pos<src.size() && depth>0){

        char c=src[pos];

        if(c=='{') depth++;
        else if(c=='}') depth--;

        if(depth>0) expr+=c;

        pos++;
    }

    return expr;
}

// --------------------------------
Lexer::Lexer(const string& src, ErrorReporter& rep) : reporter(rep) {
    currentLine = 1;
    currentColumn = 1;
    source=src;
    pos=0;
    insideJSX=false;
    jsxDepth=0;
    openBrackets=0;
    indentStack={0};
}

// --------------------------------
vector<Token> Lexer::tokenize(){

    vector<Token> tokens;

    auto peek=[&](int o=0)->char{
        if(pos+o>=source.size()) return '\0';
        return source[pos+o];
    };

    while(pos<source.size()){

        char c=peek();

        // ------------------------------------
        // NATIVE PYTHON INDENTATION TRACKING
        // ------------------------------------
        if (c == '\n') { currentLine++; currentColumn = 1; }
        if (c != '\n') currentColumn++;
        if (!insideJSX && (c == '\n' || c == '\r')) {
            pos++;
            if (c == '\r' && peek() == '\n') pos++; // Handle Windows CRLF natively
            
            int spaces = 0;
            while(pos < source.size() && (peek() == ' ' || peek() == '\t')) {
                if (peek() == '\t') spaces += 4;
                else spaces += 1;
                pos++;
            }
            
            // Ignore completely blank lines or full comment lines for indentation logic
            if (pos < source.size() && (peek() == '\n' || peek() == '\r')) continue;
            if (pos < source.size() && peek() == '#') continue;

            // Only process indentation if we are NOT inside a raw Python multiline list/dict/tuple!
            if (openBrackets == 0) {
                int currentIndent = indentStack.back();
                if (spaces > currentIndent) {
                    indentStack.push_back(spaces);
                    tokens.push_back({INDENT, "{", currentLine, currentColumn});
                } else if (spaces < currentIndent) {
                    while (indentStack.size() > 1 && indentStack.back() > spaces) {
                        indentStack.pop_back();
                        tokens.push_back({DEDENT, "}"});
                    }
                }
            }
            continue;
        }

        if(!insideJSX && isSpace(c)){
            pos++;
            continue;
        }

        // DEF
        if(source.compare(pos,3,"def")==0 && !isAlnum(peek(3))){
            tokens.push_back({DEF, "def", currentLine, currentColumn});
            pos+=3;
            continue;
        }

        // RETURN
        if(source.compare(pos,6,"return")==0 && !isAlnum(peek(6))){
            tokens.push_back({RETURN, "return", currentLine, currentColumn});
            pos+=6;
            continue;
        }

        // IMPORT
        if(source.compare(pos,6,"import")==0 && !isAlnum(peek(6))){
            tokens.push_back({IMPORT, "import", currentLine, currentColumn});
            pos+=6;
            continue;
        }

        // PYTHON COMMENTS
        if(!insideJSX && c == '#') {
            while(pos < source.size() && peek() != '\n' && peek() != '\r') pos++;
            continue;
        }
        
        // FROM
        if(source.compare(pos,4,"from")==0 && !isAlnum(peek(4))){
            tokens.push_back({FROM, "from", currentLine, currentColumn});
            pos+=4;
            continue;
        }

        // STRING
        if(peek() == '"' || peek() == '\'' || peek() == '`'){
            char quote = peek();
            bool isTriple = false;
            
            if(pos + 2 < source.size() && peek(1) == quote && peek(2) == quote) {
                isTriple = true;
                pos += 3;
            } else {
                pos++;
            }
            
            string v;
            while(pos < source.size()){
                if(isTriple){
                    if(pos + 2 < source.size() && peek() == quote && peek(1) == quote && peek(2) == quote){
                        pos += 3;
                        break;
                    }
                } else {
                    if(peek() == quote) {
                        pos++;
                        break;
                    }
                }
                
                if(peek() == '\\' && pos + 1 < source.size()){
                    v += source[pos++];
                    v += source[pos++];
                } else {
                    v += source[pos++];
                }
            }
            
            if (pos >= source.size()) {
                reporter.report("RX100", "SyntaxError", "Unterminated string literal", currentLine, currentColumn, v.length() + 1);
            }
            
            tokens.push_back({STRING, v, currentLine, currentColumn});
            continue;
        }

        // CLOSE TAG
        if(peek()=='<' && peek(1)=='/'){
            pos+=2;
            string tag;
            while(isAlnum(peek()))
                tag+=source[pos++];
            if(peek()=='>') pos++;

            tokens.push_back({TAG_CLOSE, tag, currentLine, currentColumn});

            jsxDepth--;
            if(jsxDepth<=0) insideJSX=false;
            continue;
        }

        // OPEN TAG
        if(peek()=='<' && isAlpha(peek(1))){

            pos++;
            string tag;
            while(isAlnum(peek()))
                tag+=source[pos++];

            tokens.push_back({TAG_OPEN, tag, currentLine, currentColumn});
            insideJSX=true;
            jsxDepth++;

            while(pos<source.size()){

                while(isSpace(peek())) pos++;

                if(peek()=='/' && peek(1)=='>'){
                    tokens.push_back({SELF_CLOSE, "/>", currentLine, currentColumn});
                    pos+=2;
                    jsxDepth--;
                    if(jsxDepth<=0) insideJSX=false;
                    goto next_loop;
                }

                if(peek()=='>'){
                    pos++;
                    break;
                }

                if(isAlpha(peek())){

                    string name;
                    while(isAlnum(peek()))
                        name+=source[pos++];

                    tokens.push_back({ATTRIBUTE_NAME, name, currentLine, currentColumn});

                    while(isSpace(peek())) pos++;

                    if(peek()=='='){
                        tokens.push_back({EQUAL, "=", currentLine, currentColumn});
                        pos++;
                    }

                    while(isSpace(peek())) pos++;

                    if(peek()=='"'){
                        pos++;
                        string v;
                        while(peek()!='"')
                            v+=source[pos++];
                        pos++;
                        tokens.push_back({ATTRIBUTE_VALUE, v, currentLine, currentColumn});
                    }
                    else if(peek()=='{'){
                        pos++;
                        tokens.push_back({
                            ATTRIBUTE_EXPR,
                            readExpression(source,pos),
                            currentLine,
                            currentColumn
                        });
                    }
                }
                else pos++;
            }
            continue;
        }

        // JSX EXPRESSION
        if(insideJSX && peek()=='{'){
            pos++;
            tokens.push_back({
                EXPRESSION,
                readExpression(source,pos),
                currentLine,
                currentColumn
            });
            continue;
        }

        // TEXT
        if(insideJSX && peek()!='<' && peek()!='{'){
            string text;
            while(peek()!='<' && peek()!='{' && pos<source.size())
                text+=source[pos++];

            string cleaned=normalizeJSXText(text);

            if(!cleaned.empty())
                tokens.push_back({TEXT, cleaned, currentLine, currentColumn});

            continue;
        }

        // NUMBERS
        if(isdigit(peek())){
            string num;
            while(isdigit(peek()) || peek() == '.')
                num += source[pos++];
            tokens.push_back({IDENTIFIER, num, currentLine, currentColumn});
            continue;
        }

        // IDENTIFIER + JS LITERALS ⭐ FIX
        if(isAlpha(peek())){
            string id;
            while(isAlnum(peek()) || peek() == '.' || peek() == '_')
                id += source[pos++];

            if(id=="true" || id=="True")
                tokens.push_back({TRUE, "true", currentLine, currentColumn});
            else if(id=="false" || id=="False")
                tokens.push_back({FALSE, "false", currentLine, currentColumn});
            else if(id=="null" || id=="None")
                tokens.push_back({NULLTOK, "null", currentLine, currentColumn});
            else
                tokens.push_back({IDENTIFIER, id, currentLine, currentColumn});

            continue;
        }

        if(c=='('){openBrackets++; tokens.push_back({LPAREN, "(", currentLine, currentColumn});pos++;continue;}
        if(c==')'){openBrackets--; tokens.push_back({RPAREN, ")", currentLine, currentColumn});pos++;continue;}
        if(c=='['){openBrackets++; tokens.push_back({LBRACKET, "[", currentLine, currentColumn});pos++;continue;}
        if(c==']'){openBrackets--; tokens.push_back({RBRACKET, "]", currentLine, currentColumn});pos++;continue;}
        if(c==':'){tokens.push_back({COLON, ":", currentLine, currentColumn});pos++;continue;}
        if(c==','){tokens.push_back({COMMA, ",", currentLine, currentColumn});pos++;continue;}
        if(c=='='){tokens.push_back({EQUAL, "=", currentLine, currentColumn});pos++;continue;}
        
        // Add Brace Token Parsers for valid internal JS Mapping
        if(c=='{'){openBrackets++; tokens.push_back({IDENTIFIER, "{", currentLine, currentColumn});pos++;continue;}
        if(c=='}'){openBrackets--; tokens.push_back({IDENTIFIER,"}", currentLine, currentColumn});pos++;continue;}

        // Catch all unknown valid operators (like +, -, ||, &&, ;, !, ?, @, etc)
        // If it's totally invalid, we still bundle it into an IDENTIFIER to let the parser fail,
        // OR we can explicitly flag bad characters here.
        if(!isSpace(c) && !isAlpha(c) && !isdigit(c)) {
            string op;
            while(pos < source.size() && !isSpace(peek()) && !isAlpha(peek()) && !isdigit(peek()) &&
                  peek() != '(' && peek() != ')' && peek() != '[' && peek() != ']' && 
                  peek() != '{' && peek() != '}' && peek() != ':' && peek() != ',' && peek() != '=' && peek() != '"') {
                op += source[pos++];
            }
            if(!op.empty()) {
                // If the user typed an invalid Python/JS operator like `@`
                if (op.find('@') != string::npos || op.find('$') != string::npos) {
                    reporter.report("RX101", "SyntaxError", "Invalid character: " + op, currentLine, currentColumn, op.length());
                }
                tokens.push_back({IDENTIFIER, op, currentLine, currentColumn});
                continue;
            }
        }

        pos++;

next_loop:
        continue;
    }

    // ------------------------------------
    // EOF FLUSH (Clear leftover scopes)
    // ------------------------------------
    while(indentStack.size() > 1) {
        indentStack.pop_back();
        tokens.push_back({DEDENT, "}", currentLine, currentColumn});
    }

    tokens.push_back({END, "", currentLine, currentColumn});
    return tokens;
}