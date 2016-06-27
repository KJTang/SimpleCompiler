#include <iostream>
#include <sstream>
#include <fstream>

#include "VirtualMachine.h"
#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main(int argc, char *args[]) {
    if (argc != 2) {
        std::cout<<"code file not given"<<std::endl;
        return 0;
    }
    ifstream fin(args[1]);
    std::string str((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    // VirtualMachine vm(str);
    // std::cout<<"--ReadCode:"<<std::endl;
    // std::cout<<vm.readCode()<<std::endl;
    // std::cout<<"--OutputCode:"<<std::endl;
    // vm.outputCode();
    // std::cout<<"--ExecCode:"<<std::endl;
    // vm.execCode();

    Lexer lexer(str);
    lexer.lex();
    // lexer.print();

    Parser parser(lexer.getTokens());
    parser.parse();
    parser.print();
    parser.generateAsmCode();
    std::cout<<"------------------ASM CODE------------------"<<std::endl;
    std::cout<<parser.getAsmCode()<<"<<"<<std::endl;

    return 0;
}