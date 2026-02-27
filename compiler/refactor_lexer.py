import re

with open('compiler/lexer.cpp', 'r') as f:
    text = f.read()

# Update Constructor
text = text.replace(
    "Lexer::Lexer(const string& src){",
    "Lexer::Lexer(const string& src, ErrorReporter& rep) : reporter(rep) {\n    currentLine = 1;\n    currentColumn = 1;"
)

# Replace all tokens.push_back({TOKEN, val});
text = re.sub(
    r'tokens\.push_back\(\{([A-Z_]+),\s*([^}]+)\}\);',
    r'tokens.push_back({\1, \2, currentLine, currentColumn});',
    text
)

# Intercept line/col numbers safely on specific \n bumps inside the main loop
text = text.replace(
    "if (!insideJSX && (c == '\\n' || c == '\\r')) {",
    "if (c == '\\n') { currentLine++; currentColumn = 1; }\n        if (c != '\\n') currentColumn++;\n        if (!insideJSX && (c == '\\n' || c == '\\r')) {"
)

with open('compiler/lexer.cpp', 'w') as f:
    f.write(text)

print("Lexer safely refactored.")
