#include <iostream>
#include <sstream>
#include <fstream>

#include "lexer.h"
#include "parser.h"

int main(int argc, char *args[]) {
    Lexer lexer;
    Parser parser;

    if (argc != 2) {
        std::cout<<"code file not given"<<std::endl;
        return 0;
    }
    std::ifstream fin(args[1]);
    // compiler input & lexer input
    std::string inputStr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    // lexer output & parser input
    std::vector<TokenStruct*> tokens;
    // parser output
    std::vector<ASTNode*> astnode_list;
    // compiler output
    std::string outputStr;

    lexer.Input(inputStr);
    lexer.Lex();
    lexer.Output(tokens);

    parser.Input(tokens);
    parser.Parse();
    parser.Output(astnode_list);

    return 0;
}