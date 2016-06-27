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

std::string ASTNode::eval() {
    return "";
}

std::string NumberAST::eval() {
    std::string str = "PUSH "+mValue+" ";
    return str;
}

std::string VariableAST::eval() {
    std::string str = "LOAD "+mName+" ";
    return str;
}

std::string BinaryExprAST::eval() {
    std::string str = "";
    switch (mOpr) {
        case '+': {
            str += LHS->eval() + RHS->eval() + "ADD ";
            break;
        }
        case '-': {
            str += LHS->eval() + RHS->eval() + "SUB ";
            break;
        }
        case '*': {
            str += LHS->eval() + RHS->eval() + "MUL ";
            break;
        }
        case '/': {
            str += LHS->eval() + RHS->eval() + "DIV ";
            break;
        }
        case '=': {
            str += RHS->eval() + "SAVE " + LHS->eval().substr(5);
            break;
        }
        // case ',': {
        //     str = LHS->eval() , RHS->eval();
        //     break;
        // }
        default: {
            std::cout<<"Unknown BinaryExprAST opr: "<<mOpr<<std::endl;
            break;
        }
    }
    return str;
}

std::string BlockAST::eval() {
    std::string str;
    for (auto it = statements.begin(); it != statements.end(); ++it) {
        str += (*it)->eval() + " ";
    }
    return str;
}

std::string IfExprAST::eval() {
    std::string str = condAST->eval();
    std::string label = generateJmpLabel();
    str += "JNZ " + label + " ";    // "JNZ LABEL"
    str += ifBlockAST->eval() + label + ": ";
    if (elseBlockAST) {
        str += elseBlockAST->eval();
    }
    return str;
}