#include "Parser.h"

#include <iostream>
#include <stack>

#include <cassert>

#define DEBUG

bool Parser::parse() {
    getNextToken();
    ASTNode *newStat = this->parseStatement();
    if (newStat) {
        ASTList.push_back(newStat);
    }
    

    // while (true) {
    //     switch (curToken.first) {
    //         case Token::END_OF_FILE: {
    //             return true;
    //         }
    //         case Token::END_OF_LINE: {
    //             getNextToken();
    //             break;
    //         }
    //         case Token::KEYWORD_INT: {
    //             // TODO:
    //             break;
    //         }
    //         case Token::KEYWORD_RET: {
    //             // TODO:
    //             break;
    //         }
    //         default: {
    //             ASTNode *newExpr = this->parseExpression();
    //             if (newExpr) {
    //                 ASTList.push_back(newExpr);
    //             }
    //             break;
    //         }
    //     }
    // }
    return true;
}

ASTNode *Parser::parseStatement() {
    #ifdef DEBUG
    std::cout<<curToken.second<<"\tparseStatement"<<std::endl;
    #endif
    switch (curToken.first) {
        // case Token::KEYWORD_INT:
        // case Token::KEYWORD_RET:
        case Token::KEYWORD_IF: {
            return this->parseIfExpr();
        }
        case Token::KEYWORD_WHILE: {
            return this->parseWhileExpr();
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
    ASTNode *result = new NumberAST(strtod(curToken.second.c_str(), nullptr));
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
        auto lastToken = getLastToken();
        if (lastToken.first == static_cast<Token>(')')) {
            getNextToken(); // eat ')'
            break;
        }
    }
    return new CallExprAST(identifier, args);
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
                // std::cout<<"number"<<std::endl;
                numStack.push(this->parseNumber());
                break;
            }
            case Token::IDENTIFIER: {
                // std::cout<<"identifier"<<std::endl;
                numStack.push(this->parseIdentifier());
                break;
            }
            case static_cast<Token>('('): {
                // std::cout<<"("<<std::endl;
                numStack.push(this->parseParenExpr());
                break;
            }
            case static_cast<Token>(','): {
                // expression end
                // std::cout<<","<<std::endl;
            }
            case static_cast<Token>(';'): {
                // expression end
                // std::cout<<";"<<std::endl;
                getNextToken(); // eat ',' or ';'
            }
            case static_cast<Token>(')'): {
                // expression end
                // std::cout<<")"<<std::endl;
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

void Parser::print() {
    for (auto it = ASTList.begin(); it != ASTList.end(); ++it) {
        std::cout<<"Expr Start ---"<<std::endl;
        (*it)->print();
        // (*it)->eval();
        std::cout<<"Expr End   ---"<<std::endl;
    }
}

std::string Parser::getAsmCode() {
    return asmcode;
}