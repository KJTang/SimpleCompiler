#include <iostream>
#include <sstream>
#include <fstream>

#include "err_handler.h"
#include "lexer.h"
#include "parser.h"
#include "analyser.h"

#include "converter.h"

int main(int argc, char *args[]) {
    Lexer lexer;
    Parser parser;
    Analyser analyser;

    if (argc != 2) {
        std::cout<<"code file not given"<<std::endl;
        return 0;
    }
    std::ifstream fin(args[1]);
    // compiler input & lexer input
    std::string inputStr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    // lexer output & parser input
    std::vector<TokenStruct*> tokens;
    // parser output & analyser input
    std::vector<ASTNode*> astnode_list;
    // analyser output
    std::vector<ASTNode*> analysised_list;
    // compiler output
    std::string outputStr;

    lexer.Input(inputStr);
    lexer.Lex();
    lexer.Output(tokens);

    parser.Input(tokens);
    parser.Parse();
    parser.Output(astnode_list);

    analyser.Input(astnode_list);
    analyser.Analysis();
    analyser.Output(analysised_list);

    ErrorHandler::GetInstance()->Output();

    return 0;
}