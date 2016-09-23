#pragma once

#include <string>
#include <vector>
#include <map>

#include "err_handler.h"
#include "lexer.h"
#include "astnode.h"

// operators(+ - / *) precedence
static std::map<int, int> binary_operator_precedence = {
    {'<', 1}, 
    {'>', 1}, 
    {(int)Token::OP_GTE, 1}, 
    {(int)Token::OP_LTE, 1}, 
    {(int)Token::OP_EQU, 1}, 
    {(int)Token::OP_NE, 1}, 
    {'+', 2}, 
    {'-', 2}, 
    {'*', 3}, 
    {'/', 3},
};

class Parser {
private:
    // input
    std::vector<std::pair<Token, std::string>> tokens_;
    // output
    std::vector<ASTNode*> astnode_list_;

    std::pair<Token, std::string> cur_token_;
    int pos_ = 0;
    bool err_occur_ = false;

    ASTNode* ParseStatement();
    ASTNode* ParseBlock();
    ASTNode* ParseIfExpression();
    ASTNode* ParseWhileExpression();
    ASTNode* ParseExpression();
    ASTNode* ParseParenExpression();

    ASTNode* ParseConst();
    ASTNode* ParseVariable();
    ASTNode* ParseVarArray(ASTNode* var);
    ASTNode* ParseVarMember(ASTNode* var);
    ASTNode* ParseVarFunc(ASTNode* var);
    ASTNode* ParseAssignment(ASTNode* var);
    ASTNode* ParseDefArray(ASTNode* var);
    ASTNode* ParseDefFunc(ASTNode* var);
    ASTNode* ParseDefClass();

    ASTNode* ParseNewClassOperator(ASTNode* var);
    ASTNode* ParseReturnOperator();
public:
    Parser();
    ~Parser();
    
    void Input(const std::vector<std::pair<Token, std::string>>& tokens);
    void Output(std::vector<ASTNode*>& astnode_list);
    bool Parse();
};