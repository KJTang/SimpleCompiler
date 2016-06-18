#pragma once

#include <map>

#include "Lexer.h"
#include "AST.h"

class Parser {
private:
    std::vector<std::pair<Token, std::string>> mTokens;
    std::vector<ExprAST*> ASTList;
    std::string asmcode;

    std::pair<Token, std::string> curToken;
    int pos = 0;
    std::pair<Token, std::string> &getNextToken() {
        return curToken = mTokens[pos++];
    }

    // operators(+ - / *) precedence
    std::map<char, int> BinaryOperatorPrecedence = {
        // {'<', 1}, 
        {'+', 20}, 
        {'-', 20}, 
        {'*', 40}, 
        {'/', 40}
    };
    int getOperatorPrecedence(char c) {
        return BinaryOperatorPrecedence[c];
    }
    // int getOperatorPrecedence() {
    //     if (!isascii(curToken.second[0])) {
    //         return -1;
    //     }
    //     // Make sure it's a declared binop.
    //     int oprPrec = BinaryOperatorPrecedence[curToken.second[0]];
    //     if (oprPrec <= 0) {
    //         return -1;
    //     }
    //     return oprPrec;
    // }
public:
    Parser(const std::vector<std::pair<Token, std::string>> &tokens) : mTokens(tokens) {}
    ~Parser() {
        for (int i = 0; i != ASTList.size(); ++i) {
            delete ASTList[i];
        }
    }
    
    bool parse();

    // PrimaryExpr ::= <NumberExpr> | <ParenExpr> | <IdentifierExpr>
    ExprAST *parsePrimary();
    // NumberExpr
    ExprAST *parseNumberExpr();
    // ParenExpr ::= "(" <Expression> ")"
    ExprAST *parseParenExpr();
    // IdentifierExpr ::= variable name or function
    ExprAST *parseIdentifierExpr();
    
    // Expression ::= <PrimaryExpr> {<operator> <PrimaryExpr>}
    ExprAST *parseExpression();
    ExprAST *parseBinaryOperatorRHS(int exprPrec, ExprAST *LHS);

    void print();

    std::string getAsmCode();
};