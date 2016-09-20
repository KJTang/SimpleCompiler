#pragma once

#include <string>
#include <vector>

#include "lexer.h"
#include "astnode.h"

class Parser {
private:
    // input
    std::vector<std::pair<Token, std::string>> tokens_;
    // output
    std::vector<ASTNode*> astnode_list_;
    int pos_ = 0;

    ASTNode* ParseConst();
    ASTNode* ParseVariable();
public:
    Parser();
    ~Parser();
    
    void Input(const std::vector<std::pair<Token, std::string>>& tokens);
    void Output(std::vector<ASTNode*>& astnode_list);
    bool Parse();
};