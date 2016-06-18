#pragma once

#include <string>
#include <vector>

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual void print();
    virtual double eval();
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
private:
    double mValue;
public:
    NumberExprAST(double value) : mValue(value) {}
    virtual void print();
    virtual double eval();
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
private:
    std::string mName;
public:
    VariableExprAST(const std::string &name) : mName(name) {}
    virtual void print();
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
private:
    char mOpr;
    ExprAST *LHS, *RHS;
public:
    BinaryExprAST(char opr, ExprAST *lhs, ExprAST *rhs)
        : mOpr(opr), LHS(lhs), RHS(rhs) {}
    virtual void print();
    virtual double eval();
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
private:
    std::string mCallee;
    std::vector<ExprAST*> mArgs;
public:
    CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
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
    ExprAST *mBody;
public:
    FunctionAST(PrototypeAST *proto, ExprAST *body)
        : mProto(proto), mBody(body) {}

};