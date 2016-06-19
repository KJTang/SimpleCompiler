#pragma once

#include <string>
#include <vector>

/// ASTNode - Base class for all expression nodes.
class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual void print();
    virtual double eval();
};

/// NumberAST - Expression class for numeric literals like "1.0".
class NumberAST : public ASTNode {
private:
    double mValue;
public:
    NumberAST(double value) : mValue(value) {}
    virtual void print();
    virtual double eval();
};

/// VariableAST - Expression class for referencing a variable, like "a".
class VariableAST : public ASTNode {
private:
    std::string mName;
public:
    VariableAST(const std::string &name) : mName(name) {}
    virtual void print();
    virtual double eval();
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
    virtual double eval();
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ASTNode {
private:
    std::string mCallee;
    std::vector<ASTNode*> mArgs;
public:
    CallExprAST(const std::string &callee, std::vector<ASTNode*> &args)
        : mCallee(callee), mArgs(args) {}
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