#pragma once
#include <string>
#include <vector>

struct CompileError {
    std::string code;
    std::string message;
    std::string type;
    int line;
    int column;
    int length;
};

class ErrorReporter {
public:
    void setSource(const std::string& src,
                   const std::string& filename);

    void report(const std::string& code,
                const std::string& type,
                const std::string& msg,
                int line,
                int column,
                int length = 1);

    bool hasErrors() const;
    void print() const;

private:
    std::vector<CompileError> errors;
    std::vector<std::string> sourceLines;
    std::string filename;
};