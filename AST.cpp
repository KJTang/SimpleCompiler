#include "AST.h"

#include <iostream>

void ASTNode::print() {
    //
}

void NumberAST::print() {
    std::cout<<"number: "<<mValue<<std::endl;
}

void VariableAST::print() {
    std::cout<<"var: "<<mName<<std::endl;
}

void BinaryExprAST::print() {
    LHS->print();
    std::cout<<"bin opr: "<<mOpr<<std::endl;
    RHS->print();
}

void CallExprAST::print() {
    std::cout<<"call: "<<mCallee<<std::endl;
}

double ASTNode::eval() {
    return 0;
}

double NumberAST::eval() {
    std::cout<<mValue<<std::endl;
    return mValue;
}

double VariableAST::eval() {
    std::cout<<mName<<std::endl;
    return 0;
}

double BinaryExprAST::eval() {
    double result;
    switch (mOpr) {
        case '+': {
            result = LHS->eval() + RHS->eval();
            break;
        }
        case '-': {
            result = LHS->eval() - RHS->eval();
            break;
        }
        case '*': {
            result = LHS->eval() * RHS->eval();
            break;
        }
        case '/': {
            result = LHS->eval() / RHS->eval();
            break;
        }
        case ',': {
            result = LHS->eval() , RHS->eval();
            break;
        }
        default: {
            std::cout<<"Unknown BinaryExprAST opr: "<<mOpr<<std::endl;
            break;
        }
    }
    std::cout<<result<<std::endl;
    return result;
}
