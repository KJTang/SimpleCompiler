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

void CallFuncAST::print() {
    std::cout<<"call func: "<<mCallee<<"("<<std::endl;
    for (auto it = mArgs.begin(); it != mArgs.end(); ++it) {
        (*it)->print();
    }
    std::cout<<")"<<std::endl;
}

void FunctionAST::print() {
    std::cout<<"def func "<<mFunc.second<<": (";
    for (auto it = mArgs.begin(); it != mArgs.end(); ++it) {
        std::cout<<(*it).second<<", ";
    }
    std::cout<<"nullptr) ";
    blockAST->print();
}

std::string ASTNode::eval() {
    return "";
}

std::string NumberAST::eval() {
    return "PUSH "+mValue+" ";
}

std::string VariableAST::eval() {
    return "LOAD "+mName+" ";
}

std::string AssignmentAST::eval() {
    std::string str;
    for (auto it = mVarList.begin(); it != mVarList.end(); ++it) {
        str += "INT " + static_cast<VariableAST*>(static_cast<BinaryExprAST*>(*it)->getLNode())->getName()
         + " " + (*it)->eval();
    }
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
        str += (*it)->eval();
    }
    return str;
}

std::string IfExprAST::eval() {
    std::string str = condAST->eval();
    std::string label = generateJmpLabel();
    str += "JZ " + label + " ";    // "JZ LABEL"
    str += ifBlockAST->eval() + label + ": ";
    if (elseBlockAST) {
        str += elseBlockAST->eval();
    }
    return str;
}

std::string WhileExprAST::eval() {
    std::string condLabel = generateJmpLabel();
    std::string quitLabel = generateJmpLabel();
    return condLabel + ": " + condAST->eval() + "JZ " + quitLabel + " " + blockAST->eval() + quitLabel + ": ";
}

std::string CallFuncAST::eval() {
    std::string str;
    for (auto it = mArgs.begin(); it != mArgs.end(); ++it) {
        str += (*it)->eval();
    }
    str += "CALL " + mCallee + " ";
    return str;
}

std::string ReturnAST::eval() {
    return retValue->eval() + "RET ";
}

std::string FunctionAST::eval() {
    return "FUNC " + mFunc.second + " " + blockAST->eval() + "ENDF ";
}