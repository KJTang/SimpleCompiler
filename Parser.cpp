#include "Parser.h"

#include <iostream>
#include <stack>

bool Parser::parse() {
    getNextToken();
    while (true) {
        switch (curToken.first) {
            case Token::END_OF_FILE: {
                return true;
            }
            case Token::END_OF_LINE: {
                getNextToken();
                break;
            }
            case Token::KEYWORD_INT: {
                // TODO:
                break;
            }
            case Token::KEYWORD_RET: {
                // TODO:
                break;
            }
            default: {
                ASTNode *newExpr = this->parseExpression();
                if (newExpr) {
                    ASTList.push_back(newExpr);
                }
                break;
            }
        }
    }
    return true;
}

ASTNode *Parser::parseNumber() {
    ASTNode *result = new NumberExprAST(strtod(curToken.second.c_str(), nullptr));
    getNextToken();
    return result;
}

ASTNode *Parser::parseParenExpr() {
    getNextToken();  // eat '('
    ASTNode *expr = parseExpression();
    if (curToken.first != Token::OPERATOR || curToken.second[0] != ')') {
        std::cout<<"expected \')\'"<<std::endl;
    }
    getNextToken();  // eat ')'
    return expr;
}

ASTNode *Parser::parseIdentifier() {
    std::string identifier = curToken.second;
    getNextToken();
    // variable name
    if (curToken.first != Token::OPERATOR || curToken.second[0] != '(') {
        return new VariableExprAST(identifier);
    }
    return nullptr;
    // // function
    // getNextToken();
    // std::vector<ASTNode*> args;
    // while (true) {
    //     ASTNode *arg = parseExpression();
    //     if (!arg) {
    //         return nullptr;
    //     }
    //     args.push_back(arg);
    //     if (curToken.first == Token::LABEL_L_PAREN) {
    //         break;
    //     }
    //     if (curToken.first != Token::LABEL_COMMA) {
    //         std::cout<<"Missing \',\'"<<std::endl;
    //     }
    //     getNextToken();
    // }
}

ASTNode *Parser::parseExpression() {
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
            case Token::OPERATOR: {
                // if '(''
                if (curToken.second[0] == '(') {
                    // std::cout<<"("<<std::endl;
                    numStack.push(this->parseParenExpr());
                    break;
                }
                // if ')' or ';'
                if (curToken.second[0] == ')' || curToken.second[0] == ';') {
                    if (curToken.second[0] == ';') {
                        // std::cout<<";"<<std::endl;
                        getNextToken();
                    } else {
                        // std::cout<<")"<<std::endl;
                    }
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
                // else ',' go to default
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

void Parser::print() {
    for (auto it = ASTList.begin(); it != ASTList.end(); ++it) {
        std::cout<<"Expr Start ---"<<std::endl;
        (*it)->print();
        (*it)->eval();
        std::cout<<"Expr End   ---"<<std::endl;
    }
}

std::string Parser::getAsmCode() {
    return asmcode;
}