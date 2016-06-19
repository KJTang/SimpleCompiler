#pragma once

#include <map>

#include "Lexer.h"
#include "AST.h"

class Parser {
private:
    std::vector<std::pair<Token, std::string>> mTokens;
    std::vector<ASTNode*> ASTList;
    std::string asmcode;

    std::pair<Token, std::string> curToken;
    int pos = 0;
    std::pair<Token, std::string> &getNextToken() {
        return curToken = mTokens[pos++];
    }

    // operators(+ - / *) precedence
    std::map<char, int> BinaryOperatorPrecedence = {
        {',', 0},
        {'<', 1}, 
        {'>', 1}, 
        {'+', 2}, 
        {'-', 2}, 
        {'*', 3}, 
        {'/', 3},
    };
    int getOperatorPrecedence(char c) {
        return BinaryOperatorPrecedence[c];
    }
public:
    Parser(const std::vector<std::pair<Token, std::string>> &tokens) : mTokens(tokens) {}
    ~Parser() {
        for (int i = 0; i != ASTList.size(); ++i) {
            delete ASTList[i];
        }
    }
    
    bool parse();

    // <Number>
    ASTNode *parseNumber();
    // <Identifier> ::= variable name or function name
    ASTNode *parseIdentifier();
    // <ParenExpr> ::= "(" <Expression> ")"
    ASTNode *parseParenExpr();
    // <Expression> ::= { <Number|Identifier> {<Operator> <Number|Identifier>} }
    ASTNode *parseExpression();
    // <Block> ::= "{" {<Expression> ";"} "}"
    ASTNode *parseBlock();
    // <IfExpr> ::= "if" <ParenExpr> <Block>
    ASTNode *parseIfExpr();

    // test
    void print();
    std::string getAsmCode();
};