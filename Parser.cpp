#include "Parser.h"

#include <iostream>
#include <stack>

bool Parser::parse() {
    getNextToken();
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
    switch (curToken.first) {
        // case Token::KEYWORD_INT:
        // case Token::KEYWORD_RET:
        case Token::KEYWORD_IF: {
            ASTNode *cond = parseExpression();
            ASTNode *block = parseBlock();
            return new IfExprAST(cond, block);
        }
        default: {
            return parseExpression();
        }
    }
}

ASTNode *Parser::parseNumber() {
    ASTNode *result = new NumberAST(strtod(curToken.second.c_str(), nullptr));
    getNextToken();
    return result;
}

ASTNode *Parser::parseParenExpr() {
    getNextToken();  // eat '('
    ASTNode *expr = parseExpression();
    if (curToken.first != static_cast<Token>(')')) {
        std::cout<<"expected \')\'"<<std::endl;
    }
    getNextToken();  // eat ')'
    return expr;
}

ASTNode *Parser::parseIdentifier() {
    std::string identifier = curToken.second;
    getNextToken();
    // variable name
    if (curToken.first != static_cast<Token>('(')) {
        return new VariableAST(identifier);
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
            case static_cast<Token>('('): {
                numStack.push(this->parseParenExpr());
                break;
            }
            case static_cast<Token>(';'): {
                // expression end
                getNextToken();
            }
            case static_cast<Token>(')'): {
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
    getNextToken(); // eat '{'
    std::vector<ASTNode*> exprs;
    while (curToken.first != static_cast<Token>('}')) {
        ASTNode *expr = parseExpression();
        if (!expr) {
            exprs.push_back(expr);
        }
    }
    ASTNode *block = new BlockAST(exprs);
    getNextToken(); // eat '}'
    return nullptr;
}

ASTNode *Parser::parseIfExpr() {
    return nullptr;
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