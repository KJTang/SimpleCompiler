#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "Lexer.h"

static int jmpLabel = 0;
static std::string generateJmpLabel() {
    std::ostringstream label;
    label<<"L"<<jmpLabel++;
    return label.str();
}

/// ASTNode - Base class for all expression nodes.
class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual void print();
    int line = 0;
    virtual std::string eval();
};

/// NumberAST - Expression class for numeric literals like "1.0".
class NumberAST : public ASTNode {
private:
    std::string mValue;
public:
    NumberAST(const std::string &value) : mValue(value) {}
    virtual void print();
    virtual std::string eval();
};

/// VariableAST - Expression class for referencing a variable, like "a".
class VariableAST : public ASTNode {
private:
    std::string mName;
public:
    VariableAST(const std::string &name) : mName(name) {}
    std::string &getName() {
        return mName;
    }
    virtual void print();
    virtual std::string eval();
};

/// AssignmentAST - statements like "int a = 5;"
class AssignmentAST : public ASTNode {
private:
    Token mType;
    std::vector<ASTNode*> mVarList;
public:
    AssignmentAST(Token type, const std::vector<ASTNode*> &varList) : mType(type), mVarList(varList) {}
    ~AssignmentAST() {
        for (int i = 0; i != mVarList.size(); ++i) {
            delete mVarList[i];
        }
    }
    virtual std::string eval();
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ASTNode {
private:
    int mOpr;
    ASTNode *LHS, *RHS;
public:
    BinaryExprAST(int opr, ASTNode *lhs, ASTNode *rhs)
        : mOpr(opr), LHS(lhs), RHS(rhs) {}
    ~BinaryExprAST() {
        delete LHS;
        delete RHS;
    }
    ASTNode *getLNode() {
        return LHS;
    }
    ASTNode *getRNode() {
        return RHS;
    }
    virtual void print();
    virtual std::string eval();
};

/// BlockAST - '{' statements "}"
class BlockAST : public ASTNode {
private:
    std::vector<ASTNode*> statements;
public:
    BlockAST(const std::vector<ASTNode*> &stat) : statements(stat) {}
    ~BlockAST() {
        for (int i = 0; i != statements.size(); ++i) {
            delete statements[i];
        }
    }
    virtual void print();
    virtual std::string eval();  
};

/// IfExprAST
class IfExprAST : public ASTNode {
private:
    ASTNode *condAST, *ifBlockAST, *elseBlockAST;
public:
    IfExprAST(ASTNode *cond, ASTNode *ifblock, ASTNode *elseblock) : 
    condAST(cond), ifBlockAST(ifblock), elseBlockAST(elseblock) {}
    ~IfExprAST() {
        delete condAST;
        delete ifBlockAST;
        if (elseBlockAST) {
            delete elseBlockAST;
        }
    }
    virtual void print();
    virtual std::string eval();
};

/// WhileExprAST
class WhileExprAST : public ASTNode {
private:
    ASTNode *condAST, *blockAST;
public:
    WhileExprAST(ASTNode *cond, ASTNode *block) : condAST(cond), blockAST(block) {}
    ~WhileExprAST() {
        delete condAST;
        delete blockAST;
    }
    virtual void print();
    virtual std::string eval();
};

/// StatementAST
class StatementAST : public ASTNode {
public:
    StatementAST() {}
    ~StatementAST() {}
    
};

/// CallFuncAST - Expression class for function calls.
class CallFuncAST : public ASTNode {
private:
    std::string mCallee;
    std::vector<ASTNode*> mArgs;
public:
    CallFuncAST(const std::string &callee, const std::vector<ASTNode*> &args)
        : mCallee(callee), mArgs(args) {}
    ~CallFuncAST() {
        for (int i = 0; i != mArgs.size(); ++i) {
            delete mArgs[i];
        }
    }
    virtual void print();
    virtual std::string eval();
};

/// ReturnAST - "return" keyword in function definition
class ReturnAST : public ASTNode {
private:
    ASTNode *retValue;
public:
    ReturnAST(ASTNode *value) : retValue(value) {}
    ~ReturnAST() {
        delete retValue;
    }
    virtual std::string eval();
};

/// FunctionAST
class FunctionAST : public ASTNode {
private:
    std::pair<Token, std::string> mFunc;
    std::vector<std::pair<Token, std::string>> mArgs;
    ASTNode *blockAST;
public:
    FunctionAST(const std::pair<Token, std::string> &func, const std::vector<std::pair<Token, std::string>> &args, ASTNode *block)
        : mFunc(func), mArgs(args), blockAST(block) {}
    ~FunctionAST() {
        delete blockAST;
    }
    virtual void print();
    virtual std::string eval();
};
