#pragma once

#include <string>
#include <vector>
#include <map>

#include "err_handler.h"
#include "lexer.h"
#include "astnode.h"

// operators(+ - / *) precedence
static std::map<int, int> binary_operator_precedence = {
    {(int)Token::OP_AND, 1}, 
    {(int)Token::OP_OR, 1}, 
    {'<', 2}, 
    {'>', 2}, 
    {(int)Token::OP_GTE, 2}, 
    {(int)Token::OP_LTE, 2}, 
    {(int)Token::OP_EQU, 2}, 
    {(int)Token::OP_NE, 2}, 
    {'+', 3}, 
    {'-', 3}, 
    {'*', 4}, 
    {'/', 4},
};

class Parser {
private:
    // input
    std::vector<TokenStruct*> tokens_;
    // output
    std::vector<ASTNode*> astnode_list_;

    TokenStruct* cur_token_;
    int pos_ = 0;
    bool err_occur_ = false;

    // count block {}
    int block_count_ = 0;

    ASTNode* ParseStatement();
    ASTNode* ParseBlock();

    ASTNode* ParseExpression();
    ASTNode* ParseExpressionIf();
    ASTNode* ParseExpressionWhile();
    ASTNode* ParseExpressionParen();
    ASTNode* ParseExpressionAssignment(ASTNode* var);

    ASTNode* ParseConst();
    ASTNode* ParseVariable();

    ASTNode* ParseCallArray(ASTNode* var);
    ASTNode* ParseCallMember(ASTNode* var);
    ASTNode* ParseCallFunc(ASTNode* var);

    ASTNode* ParseDeclareVar();

    ASTNode* ParseDefArray();
    ASTNode* ParseDefFunc();
    ASTNode* ParseDefTable();
    // ASTNode* ParseDefClass();

    ASTNode* ParseOperatorNew(ASTNode* var);
    ASTNode* ParseOperatorReturn();
    ASTNode* ParseOperatorBreak();
public:
    Parser();
    ~Parser();
    
    void Input(const std::vector<TokenStruct*>& tokens);
    void Output(std::vector<ASTNode*>& astnode_list);
    bool Parse();
};