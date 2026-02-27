#include "errors.h"
#include <iostream>
#include <sstream>

void ErrorReporter::setSource(
    const std::string& src,
    const std::string& file)
{
    filename = file;
    sourceLines.clear();

    std::stringstream ss(src);
    std::string line;

    while (std::getline(ss, line))
        sourceLines.push_back(line);
}

void ErrorReporter::report(
    const std::string& code,
    const std::string& type,
    const std::string& msg,
    int line,
    int column,
    int length)
{
    errors.push_back({code, msg, type, line, column, length});
}

bool ErrorReporter::hasErrors() const {
    return !errors.empty();
}

void ErrorReporter::print() const {

    for (const auto& e : errors) {

        int displayLine = e.line;
        int displayCol = e.column;
        
        // Clamp to last valid line for EOF errors
        if (displayLine > sourceLines.size()) {
            displayLine = sourceLines.size();
            displayCol = sourceLines.back().size() + 1; 
        }

        std::cout << "File \"" << filename
                  << "\", line " << displayLine << "\n\n";

        if (displayLine > 0 && displayLine <= sourceLines.size()) {

            const std::string& code = sourceLines[displayLine - 1];

            std::cout << "    " << code << "\n";
            std::cout << "    ";

            int spaces = displayCol - 1;
            if (spaces > code.size()) spaces = code.size();
            
            for (int i = 0; i < spaces; ++i)
                std::cout << " ";

            int printLen = e.length;
            if (printLen <= 0) printLen = 1;
            
            // +1 to allow pointing to the newline at EOF
            if (spaces + printLen > code.size() + 1) {
                printLen = code.size() - spaces + 1;
            }
            if (printLen <= 0) printLen = 1;

            for (int i = 0; i < printLen; ++i)
                std::cout << "^";
            
            std::cout << "\n\n";
        }

        std::cout << e.code << ": " << e.type << ": "
                  << e.message << "\n\n";
    }
}