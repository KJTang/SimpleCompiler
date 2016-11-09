#pragma once

#include "astnode.h"

class Calculator {
private:
    static Calculator* shared_ptr_;

    ASTNode* ValueCalculate(int line, int op, ASTNode* l, ASTNode* r);
    ASTNode* ValueCalculateInt(int line, int op, ASTNode* l, ASTNode* r);
    ASTNode* ValueCalculateDouble(int line, int op, ASTNode* l, ASTNode* r);
    ASTNode* ValueCalculateString(int line, int op, ASTNode* l, ASTNode* r);
    ASTNode* LogicCalculate(int line, int op, ASTNode* l, ASTNode* r);
    ASTNode* ConditionCalculate(int line, int op, ASTNode* l, ASTNode* r);
public:
    Calculator() {}
    ~Calculator() {}
    
    static Calculator* GetInstance() {
        if (!shared_ptr_) {
            shared_ptr_ = new Calculator();
        }
        return shared_ptr_;
    }

    ASTNode* Calculate(int line, int op, ASTNode* l, ASTNode* r);
};