#include "AST.h"

#include <iostream>

// instructions' end
const std::string insEnd = "\n";

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
    return "PUSH "+mValue+insEnd;
}

std::string VariableAST::eval() {
    return "LOAD "+mName+insEnd;
}

std::string AssignmentAST::eval() {
    std::string str;
    for (auto it = mVarList.begin(); it != mVarList.end(); ++it) {
        str += "INT " + static_cast<VariableAST*>(static_cast<BinaryExprAST*>(*it)->getLNode())->getName()
         + insEnd + (*it)->eval();
    }
    return str;
}

std::string BinaryExprAST::eval() {
    std::string str = "";
    switch (mOpr) {
        case '+': {
            str += LHS->eval() + RHS->eval() + "ADD" + insEnd;
            break;
        }
        case '-': {
            str += LHS->eval() + RHS->eval() + "SUB" + insEnd;
            break;
        }
        case '*': {
            str += LHS->eval() + RHS->eval() + "MUL" + insEnd;
            break;
        }
        case '/': {
            str += LHS->eval() + RHS->eval() + "DIV" + insEnd;
            break;
        }
        case '=': {
            str += RHS->eval() + "SAVE " + LHS->eval().substr(5);
            break;
        }
        case '>': {
            str += LHS->eval() + RHS->eval() + "GT" + insEnd;
            break;
        }
        case '<': {
            str += LHS->eval() + RHS->eval() + "LT" + insEnd;
            break;
        }
        case (int)Token::OP_GTE: {
            str += LHS->eval() + RHS->eval() + "GTE" + insEnd;
            break;
        }
        case (int)Token::OP_LTE: {
            str += LHS->eval() + RHS->eval() + "LTE" + insEnd;
            break;
        }
        case (int)Token::OP_EQU: {
            str += LHS->eval() + RHS->eval() + "EQU" + insEnd;
            break;
        }
        case (int)Token::OP_NE: {
            str += LHS->eval() + RHS->eval() + "NE" + insEnd;
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
    str += "JZ " + label + insEnd;    // "JZ LABEL"
    str += ifBlockAST->eval() + label + ":" + insEnd;
    if (elseBlockAST) {
        str += elseBlockAST->eval();
    }
    return str;
}

std::string WhileExprAST::eval() {
    std::string condLabel = generateJmpLabel();
    std::string quitLabel = generateJmpLabel();
    return condLabel + ":" + insEnd + condAST->eval() + "JZ " + quitLabel + insEnd + 
        blockAST->eval() + "JMP " + condLabel + insEnd + quitLabel + ":" + insEnd;
}

std::string CallFuncAST::eval() {
    std::string str;
    if (mCallee == "input") {   // system func "input(int)"
        str += "SCAN " + mArgs[0]->eval().substr(5);
    } else if (mCallee == "output") {   // system func "output(int)"
        str += mArgs[0]->eval() + "PRIT" + insEnd;
    } else {
        for (auto it = mArgs.begin(); it != mArgs.end(); ++it) {
            str += (*it)->eval();
        }
        str += "CALL " + mCallee + insEnd;
    }
    return str;
}

std::string ReturnAST::eval() {
    return retValue->eval() + "RET" + insEnd;
}

std::string FunctionAST::eval() {
    std::string argsStr;
    for (auto it = mArgs.rbegin(); it != mArgs.rend(); ++it) {
        // TODO: only "int" here, maybe more type in the future
        if (it->first == Token::TYPE_INT) {
            argsStr += "INT " + it->second + insEnd + "SAVE " + it->second + insEnd;
        }
    }
    return "FUNC " + mFunc.second + insEnd + argsStr + blockAST->eval() + "ENDF" + insEnd;
}