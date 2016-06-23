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

void BlockAST::print() {
    std::cout<<"block: {"<<std::endl;
    for (auto it = statements.begin(); it != statements.end(); ++it) {
        (*it)->print();
    }
    std::cout<<"}"<<std::endl;
}

void IfExprAST::print() {
    std::cout<<"if\t";
    condAST->print();
    ifBlockAST->print();
    if (elseBlockAST) {
        std::cout<<"else\t";
        elseBlockAST->print();
    }
}

void WhileExprAST::print() {
    std::cout<<"while\t";
    condAST->print();
    blockAST->print();
}

void CallExprAST::print() {
    std::cout<<"func: "<<mCallee<<"("<<std::endl;
    for (auto it = mArgs.begin(); it != mArgs.end(); ++it) {
        (*it)->print();
    }
    std::cout<<")"<<std::endl;
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
        // case ',': {
        //     result = LHS->eval() , RHS->eval();
        //     break;
        // }
        default: {
            std::cout<<"Unknown BinaryExprAST opr: "<<mOpr<<std::endl;
            break;
        }
    }
    std::cout<<result<<std::endl;
    return result;
}
