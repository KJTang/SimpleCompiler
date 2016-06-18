#include "AST.h"

#include <iostream>

void ExprAST::print() {
    //
}

void NumberExprAST::print() {
    std::cout<<"number: "<<mValue<<std::endl;
}

void VariableExprAST::print() {
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


double ExprAST::eval() {
    return 0;
}

double NumberExprAST::eval() {
    return mValue;
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
    }
    std::cout<<result<<std::endl;
    return result;
}
