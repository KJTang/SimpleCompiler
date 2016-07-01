#include <iostream>
#include <sstream>
#include <fstream>

#include "Lexer.h"
#include "Parser.h"
#include "Assembler.h"
#include "VirtualMachine.h"

int main(int argc, char *args[]) {
    if (argc != 2) {
        std::cout<<"code file not given"<<std::endl;
        return 0;
    }
    std::ifstream fin(args[1]);
    std::string str((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    Lexer lexer(str);
    lexer.lex();
    // lexer.debug();

    Parser parser(lexer.getTokens());
    parser.parse();
    // parser.debug();

    Assembler assembler(parser.getAsmCode());
    assembler.assemble();
    // assembler.debug();

    VirtualMachine vm(assembler.getBinCode());
    vm.execute();
    return 0;
}