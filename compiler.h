#pragma once

#include <iostream>

#include "lexer.h"
// #include "Parser.h"

class Compiler {
private:
    Lexer lexer;
    // Parser parser;

    std::vector<std::pair<Token, std::string>> tokens;

public:
    Compiler();
    ~Compiler();
    
    void Input(const std::string& input);
    void Output(std::string& output);
    bool Compile();
};