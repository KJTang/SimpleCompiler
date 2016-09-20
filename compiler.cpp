#include "compiler.h"

Compiler::Compiler() {}

Compiler::~Compiler() {}

void Compiler::Input(const std::string& input) {
    lexer.Input(input);
}

void Compiler::Output(std::string& output) {
    lexer.Output(tokens);
}

bool Compiler::Compile() {
    lexer.Lex();
    return true;
}