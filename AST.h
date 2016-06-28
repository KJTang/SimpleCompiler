#pragma once

#include <sstream>
#include <string>
#include <vector>

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
    // double mValue;
    std::string mValue;
public:
    // NumberAST(double value) : mValue(value) {}
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
    virtual void print();
    virtual std::string eval();
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ASTNode {
private:
    char mOpr;
    ASTNode *LHS, *RHS;
public:
    BinaryExprAST(char opr, ASTNode *lhs, ASTNode *rhs)
        : mOpr(opr), LHS(lhs), RHS(rhs) {}
    ~BinaryExprAST() {
        delete LHS;
        delete RHS;
    }
    virtual void print();
    virtual std::string eval();
};

/// BlockAST
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

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ASTNode {
private:
    std::string mCallee;
    std::vector<ASTNode*> mArgs;
public:
    CallExprAST(const std::string &callee, std::vector<ASTNode*> &args)
        : mCallee(callee), mArgs(args) {}
    ~CallExprAST() {
        for (int i = 0; i != mArgs.size(); ++i) {
            delete mArgs[i];
        }
    }
    virtual void print();
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
private:
    std::string mName;
    std::vector<std::string> mArgs;
public:
    PrototypeAST(const std::string &name, const std::vector<std::string> &args)
        : mName(name), mArgs(args) {}

};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
private:
    PrototypeAST *mProto;
    ASTNode *mBody;
public:
    FunctionAST(PrototypeAST *proto, ASTNode *body)
        : mProto(proto), mBody(body) {}

};