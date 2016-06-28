#pragma once

#include <map>

#include "Lexer.h"
#include "AST.h"

class Parser {
private:
    std::vector<std::pair<Token, std::string>> mTokens;
    std::vector<ASTNode*> ASTList;
    int asmline = 0;
    std::string asmcode;

    std::pair<Token, std::string> curToken;
    int pos = 0;
    std::pair<Token, std::string> &getNextToken() {
        return curToken = mTokens[pos++];
    }
    std::pair<Token, std::string> &lookAheadToken(int offset) {
        return mTokens[pos+offset-1];
    }

    // operators(+ - / *) precedence
    std::map<char, int> BinaryOperatorPrecedence = {
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

    // <Statement> ::= <Expression> | <Block> | <IfExpr> 
    ASTNode *parseStatement();
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
    // <WhileExpr> ::= "while" <Expression> <Block>
    ASTNode *parseWhileExpr();
    // <AssignExpr> ::= <TypeName> <Identifier>
    ASTNode *parseAssignExpr();
    // 
    ASTNode *parseDeclaration();
    ASTNode *parseFuncDeclaration();
    ASTNode *parseVarDeclaration();
    // 
    ASTNode *parseFuncReturn();

    // test
    void print();
    bool generateAsmCode();
    std::string &getAsmCode() {
        return asmcode;
    }
};