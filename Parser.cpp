#include "Parser.h"

#include <iostream>
#include <stack>

bool Parser::parse() {
    getNextToken();
    auto result = this->parseExpression();
    result->eval();
    // while (true) {
    //     switch (curToken.first) {
    //         case Token::END_OF_FILE: {
    //             // std::cout<<"EOF: "<<curToken.second<<std::endl;
    //             return true;
    //         }
    //         case Token::LABEL_SEMICOLON: 
    //         case Token::END_OF_LINE: {
    //             // std::cout<<"Next: "<<curToken.second<<std::endl;
    //             getNextToken();
    //             break;
    //         }
    //         default: {
    //             // std::cout<<"Default: "<<curToken.second<<std::endl;
    //             ExprAST *astnode = parsePrimary();
    //             // ASTList.push_back(astnode);
    //             // if (astnode) {
    //             //     astnode->print();
    //             // }
    //             break;
    //         }
    //     }
    //     // fprintf(stderr, "ready> ");
    //     // switch (CurTok) {
    //     // case tok_eof:    return;
    //     // case ';':        getNextToken(); break;  // ignore top-level semicolons.
    //     // case tok_def:    HandleDefinition(); break;
    //     // case tok_extern: HandleExtern(); break;
    //     // default:         HandleTopLevelExpression(); break;
    //     // }
    // }
    return true;
}

ExprAST *Parser::parsePrimary() {
    switch (curToken.first) {
        case Token::UNKNOWN: {
            std::cout<<"Unknown Token!!!"<<std::endl;
            return nullptr;
        }
        case Token::LABEL_SEMICOLON:
        case Token::END_OF_LINE: {
            return nullptr;
        }
        case Token::IDENTIFIER: {
            return parseIdentifierExpr();
        }
        case Token::NUMBER: {
            return parseNumberExpr();
        }
        case Token::LABEL_L_PAREN: {
            return parseParenExpr();
        }
    }
}

ExprAST *Parser::parseNumberExpr() {
    ExprAST *result = new NumberExprAST(strtod(curToken.second.c_str(), nullptr));
    getNextToken();
    return result;
}

ExprAST *Parser::parseParenExpr() {
    getNextToken();  // eat '('
    ExprAST *expr = parseExpression();
    if (!expr) {
        return nullptr;
    }

    if (curToken.first != Token::LABEL_R_PAREN) {
        std::cout<<"expected \')\'"<<std::endl;
    }
    getNextToken();  // eat ')'
    return expr;
}

ExprAST *Parser::parseIdentifierExpr() {
    std::string identifier = curToken.second;
    getNextToken();
    // variable name
    if (curToken.first != Token::LABEL_L_PAREN) {
        return new VariableExprAST(identifier);
    }
    return nullptr;
    // // function
    // getNextToken();
    // std::vector<ExprAST*> args;
    // while (true) {
    //     ExprAST *arg = parseExpression();
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

ExprAST *Parser::parseExpression() {
    std::stack<ExprAST*> numStack;
    std::stack<char> opStack;
    while (true) {
        switch (curToken.first) {
            // case Token::LABEL_COMMA:
            case Token::LABEL_SEMICOLON:
            case Token::LABEL_R_PAREN: {
                std::cout<<"return"<<std::endl;
                while (!opStack.empty()) {
                    char op = opStack.top();
                    opStack.pop();
                    ExprAST *l, *r;
                    r = numStack.top();
                    numStack.pop();
                    l = numStack.top();
                    numStack.pop();
                    BinaryExprAST *binExpr = new BinaryExprAST(op, l, r);
                    numStack.push(binExpr);
                }
                return numStack.top();
            }
            case Token::NUMBER: {
                std::cout<<"number"<<std::endl;
                numStack.push(this->parseNumberExpr());
                break;
            }
            case Token::LABEL_L_PAREN: {
                std::cout<<"("<<std::endl;
                numStack.push(this->parseParenExpr());
                break;
            }
            default: {
                std::cout<<"op"<<std::endl;
                if (!opStack.empty() && 
                    getOperatorPrecedence(opStack.top()) >= getOperatorPrecedence(curToken.second[0])) {
                    char op = opStack.top();
                    opStack.pop();
                    ExprAST *l, *r;
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


    // ExprAST *LHS = parsePrimary();
    // if (!LHS) {
    //     return nullptr;
    // }
    // return parseBinaryOperatorRHS(0, LHS);
}

// ExprAST *Parser::parseBinaryOperatorRHS(int exprPrec, ExprAST *LHS) {
//     // If this is a binary operator, find its precedence.
//     while (true) {
//         int oprPrec = getOperatorPrecedence();
//         // If this is a binary operator that binds at least as tightly as the current binary operator,
//         // consume it, otherwise we are done.
//         if (oprPrec < exprPrec){
//             return LHS;
//         }

//         // Okay, we know this is a binary operator.
//         int binOpr = curToken.second[0];
//         getNextToken();  // eat binary operator

//         // Parse the primary expression after the binary operator.
//         ExprAST *RHS = parsePrimary();
//         if (!RHS) {
//             return nullptr;
//         }

//         // If binOpr binds less tightly with RHS than the operator after RHS, let
//         // the pending operator take RHS as its LHS.
//         int nextPrec = getOperatorPrecedence();
//         if (oprPrec < nextPrec) {
//             RHS = parseBinaryOperatorRHS(oprPrec + 1, RHS);
//             if (!RHS) {
//                 return nullptr;
//             }
//         }

//         // Merge LHS/RHS.
//         LHS = new BinaryExprAST(binOpr, LHS, RHS);
//     }
// }

void Parser::print() {
    for (auto it = ASTList.begin(); it != ASTList.end(); ++it) {
        (*it)->print();
    }
}

std::string Parser::getAsmCode() {
    return asmcode;
}