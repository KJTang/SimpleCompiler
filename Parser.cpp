#include "Parser.h"

#include <iostream>
#include <stack>

// #define DEBUG

bool Parser::parse() {
    getNextToken(); // start parsing
    ASTNode *newStat = nullptr;
    while (curToken.first != Token::END_OF_FILE) {
        newStat = this->parseStatement();
        if (newStat) {
            ASTList.push_back(newStat);
        }
    }
    for (auto it = ASTList.begin(); it != ASTList.end(); ++it) {
        #ifdef DEBUG
        std::string temp = (*it)->eval();
        std::cout<<temp<<"<<"<<std::endl;
        asmcode += temp;
        #else
        asmcode += (*it)->eval();
        #endif
    }
    return true;
}

ASTNode *Parser::parseStatement() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseStatement"<<std::endl;
    #endif
    switch (curToken.first) {
        case Token::KEYWORD_IF: {
            return this->parseIfExpr();
        }
        case Token::KEYWORD_WHILE: {
            return this->parseWhileExpr();
        }
        case Token::TYPE_INT: {
            return this->parseDeclaration();
        }
        case Token::KEYWORD_RET: {
            return this->parseFuncReturn();
        }
        default: {
            return this->parseExpression();
        }
    }
}

ASTNode *Parser::parseNumber() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseNumber"<<std::endl;
    #endif
    // ASTNode *result = new NumberAST(strtod(curToken.second.c_str(), nullptr));
    ASTNode *result = new NumberAST(curToken.second);
    getNextToken();
    return result;
}

ASTNode *Parser::parseParenExpr() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseParenExpr"<<std::endl;
    #endif
    getNextToken();  // eat '('
    ASTNode *expr = this->parseExpression();
    if (curToken.first != static_cast<Token>(')')) {
        std::cout<<"expected \')\'"<<std::endl;
    }
    getNextToken();  // eat ')'
    return expr;
}

ASTNode *Parser::parseIdentifier() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseIdentifier"<<std::endl;
    #endif
    std::string identifier = curToken.second;
    getNextToken();
    // variable name
    if (curToken.first != static_cast<Token>('(')) {
        return new VariableAST(identifier);
    }
    // function call
    getNextToken(); // eat '('
    std::vector<ASTNode*> args;
    while (true) {
        ASTNode *arg = parseExpression();
        if (arg) {
            args.push_back(arg);
        }
        if (curToken.first == static_cast<Token>(')')) {
            getNextToken(); // eat ')'
            break;
        }
    }
    return new CallFuncAST(identifier, args);
}

ASTNode *Parser::parseExpression() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseExpression"<<std::endl;
    #endif
    std::stack<ASTNode*> numStack;
    std::stack<char> opStack;
    while (true) {
        switch (curToken.first) {
            case Token::NUMBER: {
                numStack.push(this->parseNumber());
                break;
            }
            case Token::IDENTIFIER: {
                numStack.push(this->parseIdentifier());
                break;
            }
            case static_cast<Token>('('): {
                numStack.push(this->parseParenExpr());
                break;
            }
            case static_cast<Token>(','): 
            case static_cast<Token>(';'): {
                // expression end
                getNextToken(); // eat ',' or ';'
            }
            case static_cast<Token>(')'): {
                // expression end
                while (!opStack.empty()) {
                    char op = opStack.top();
                    opStack.pop();
                    ASTNode *l, *r;
                    r = numStack.top();
                    numStack.pop();
                    l = numStack.top();
                    numStack.pop();
                    BinaryExprAST *binExpr = new BinaryExprAST(op, l, r);
                    numStack.push(binExpr);
                }
                if (!numStack.empty()) {
                    return numStack.top();
                } else {
                    // std::cout<<"empty expression!"<<std::endl;
                    return nullptr;
                }
            }
            default: {
                // std::cout<<"op"<<std::endl;
                if (!opStack.empty() && 
                    getOperatorPrecedence(opStack.top()) >= getOperatorPrecedence(curToken.second[0])) {
                    char op = opStack.top();
                    opStack.pop();
                    ASTNode *l, *r;
                    r = numStack.top();
                    numStack.pop();
                    l = numStack.top();
                    numStack.pop();
                    BinaryExprAST *binExpr = new BinaryExprAST(op, l, r);
                    numStack.push(binExpr);
                }
                opStack.push(curToken.second[0]);
                getNextToken();
                break;
            }
        }
    }
}

ASTNode *Parser::parseBlock() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseBlock"<<std::endl;
    #endif
    getNextToken(); // eat '{'
    std::vector<ASTNode*> stats;
    while (curToken.first != static_cast<Token>('}')) {
        ASTNode *newStat = this->parseStatement();
        if (newStat) {
            stats.push_back(newStat);
        }
    }
    ASTNode *block = new BlockAST(stats);
    getNextToken(); // eat '}'
    return block;
}

ASTNode *Parser::parseIfExpr() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseIfExpr"<<std::endl;
    #endif
    getNextToken(); // eat "if"
    ASTNode *cond = this->parseParenExpr();
    ASTNode *ifblock = this->parseBlock();
    ASTNode *elseblock = nullptr;
    if (curToken.second == "else") {
        getNextToken(); // eat "else"
        if (curToken.second == "if") {  // "else if"
            elseblock = this->parseIfExpr();
        } else {
            elseblock = this->parseBlock();   // "else {}"
        }
    }
    ASTNode *ifExpr = new IfExprAST(cond, ifblock, elseblock);
    return ifExpr;
}

ASTNode *Parser::parseWhileExpr() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseWhileExpr"<<std::endl;
    #endif
    getNextToken(); // eat "while"
    ASTNode *cond = this->parseParenExpr();
    ASTNode *block = this->parseBlock();
    ASTNode *whileExpr = new WhileExprAST(cond, block);
    return whileExpr;
}

ASTNode *Parser::parseDeclaration() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseDeclaration"<<std::endl;
    #endif
    Token aheadToken = lookAheadToken(2).first;
    // function declaration
    if (aheadToken == static_cast<Token>('(')) {
        return this->parseFuncDeclaration();
    }
    // variable declaration
    else {
        return this->parseVarDeclaration();
    }
}

ASTNode *Parser::parseVarDeclaration() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseVarDeclaration"<<std::endl;
    #endif
    Token type = curToken.first;
    getNextToken(); // eat type name
    std::vector<ASTNode*> varList;
    while (true) {
        ASTNode *var = this->parseIdentifier();
        if (curToken.first == static_cast<Token>(',')) {
            getNextToken();
            varList.push_back(new BinaryExprAST('=', var, new NumberAST("0")));
        } else if (curToken.first == static_cast<Token>('=')) {
            getNextToken();
            ASTNode *expr = this->parseExpression();
            varList.push_back(new BinaryExprAST('=', var, expr));
            if (lookAheadToken(-1).first == static_cast<Token>(';')) {
                break;
            }
        } else if (curToken.first == static_cast<Token>(';')) {
            getNextToken();
            varList.push_back(new BinaryExprAST('=', var, new NumberAST("0")));
            break;
        }
    }
    return new AssignmentAST(type, varList);   
}

ASTNode *Parser::parseFuncDeclaration() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseFuncDeclaration"<<std::endl;
    #endif
    Token type = curToken.first;
    getNextToken(); // eat function return value type
    std::string name = curToken.second;
    getNextToken(); // eat function name
    std::vector<std::pair<Token, std::string>> args;
    // if function has argument
    if (lookAheadToken(1).first != static_cast<Token>(')')) {
        do {
            getNextToken(); // eat '(' or ','
            Token varType = curToken.first;
            getNextToken(); // eat type name
            std::string varName = curToken.second;
            getNextToken(); // eat arg name
            args.push_back(std::make_pair(varType, varName));
        } while (curToken.first != static_cast<Token>(')'));
    }
    // else function has no argument
    else {
        getNextToken(); // eat '('
    }
    getNextToken(); // eat ')'
    ASTNode *block = this->parseBlock();
    return new FunctionAST(std::make_pair(type, name), args, block);
}

ASTNode *Parser::parseFuncReturn() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseFuncReturn"<<std::endl;
    #endif
    getNextToken(); // eat "return"
    ASTNode *retExpr = this->parseExpression();
    if (!retExpr) {
        retExpr = new NumberAST("0");
    }
    return new ReturnAST(retExpr);
}

void Parser::print() {
    for (auto it = ASTList.begin(); it != ASTList.end(); ++it) {
        std::cout<<"Expr Start ---"<<std::endl;
        (*it)->print();
        std::cout<<"Expr End   ---"<<std::endl;
    }
}
