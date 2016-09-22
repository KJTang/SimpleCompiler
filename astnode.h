#pragma once

#include <string>
#include <vector>

#include "lexer.h"

enum class ASTTYPE {
    DEFAULT,
    VARIABLE,
    CONST_NULL,
    CONST_BOOL,
    CONST_INT,
    CONST_DOUBLE,
    CONST_STRING,
    FUNCTION,
    CLASS,
};

class ASTNode {
private:
    ASTTYPE type_ = ASTTYPE::DEFAULT;
    std::string value_ = "null";
public:
    ASTNode() {}
    ~ASTNode() {}

    void set_type(ASTTYPE type) { type_ = type; }
    ASTTYPE get_type() { return type_; }

    void set_value(const std::string& value) { value_ = value; }
    std::string get_value() { return value_; }

    // Test
    virtual void print() {}
};

class ASTBlock : public ASTNode {
private:
    std::vector<ASTNode*> statements_;
public:
    ASTBlock(const std::vector<ASTNode*>& statments) : statements_(statments) {
        set_type(ASTTYPE::DEFAULT);
    }
    ~ASTBlock() {}

    // Test
    virtual void print() {
        std::cout<<"ASTBlock: \t"<<"{"<<statements_.size()<<"}"<<std::endl;
        for (int i = 0; i != statements_.size(); ++i) {
            statements_[i]->print();
        }
    }
};

class ASTVariable : public ASTNode {
public:
    ASTVariable(const std::string& var_name) {
        set_type(ASTTYPE::VARIABLE);
        set_value(var_name);
    }
    ~ASTVariable() {}

    // Test
    virtual void print() {
        std::cout<<"ASTVariable: \t"<<get_value()<<std::endl;
    }
};

class ASTVarArray : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* expr_;
public:
    ASTVarArray(ASTNode* var, ASTNode* expr) : var_(var), expr_(expr) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTVarArray() {}

    // Test
    virtual void print() {
        std::cout<<"ASTVarArray: \t"<<"[]"<<std::endl;
        var_->print();
        expr_->print();
    }
};

class ASTVarMember : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* member_;
public:
    ASTVarMember(ASTNode* var, ASTNode* member) : var_(var), member_(member) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTVarMember() {}

    // Test
    virtual void print() {
        std::cout<<"ASTVarMember: \t"<<"."<<std::endl;
        var_->print();
        member_->print();
    }
};

class ASTVarFunc : public ASTNode {
private:
    ASTNode* var_;
    std::vector<ASTNode*> parameters_;
public:
    ASTVarFunc(ASTNode* var, const std::vector<ASTNode*>& parameters) : var_(var), parameters_(parameters) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTVarFunc() {}

    // Test
    virtual void print() {
        std::cout<<"ASTVarFunc: \t"<<"("<<parameters_.size()<<")"<<std::endl;
        var_->print();
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->print();
        }
    }
};

class ASTConst : public ASTNode {
public:
    ASTConst(Token token, const std::string& value) {
        switch (token) {
            case Token::CONST_NULL: {
                set_type(ASTTYPE::CONST_NULL);
                break;
            }
            case Token::CONST_BOOL: {
                set_type(ASTTYPE::CONST_BOOL);
                break;
            }
            case Token::CONST_INT: {
                set_type(ASTTYPE::CONST_INT);
                break;
            }
            case Token::CONST_DOUBLE: {
                set_type(ASTTYPE::CONST_DOUBLE);
                break;
            }
            case Token::CONST_STRING: {
                set_type(ASTTYPE::CONST_STRING);
                break;
            }
        }
        set_value(value);
    }
    ~ASTConst() {}

    // Test
    virtual void print() {
        std::cout<<"ASTConst: \t"<<get_value()<<std::endl;
    }
};

class ASTBinaryOp : public ASTNode {
private:
    int op_;
    ASTNode* l_node_;
    ASTNode* r_node_;
public:
    ASTBinaryOp(int op, ASTNode* l_node, ASTNode* r_node) : op_(op), l_node_(l_node), r_node_(r_node) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTBinaryOp() {}

    // Test
    virtual void print() {
        std::cout<<"ASTBinaryOp: \t"<<(char)op_<<std::endl;
        l_node_->print();
        r_node_->print();
    }
};

class ASTAssign : public ASTNode {
private:
    ASTNode* l_node_;
    ASTNode* r_node_;
public:
    ASTAssign(ASTNode* l_node, ASTNode* r_node) : l_node_(l_node), r_node_(r_node) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTAssign() {}

    // Test
    virtual void print() {
        std::cout<<"ASTAssign: \t"<<"="<<std::endl;
        l_node_->print();
        r_node_->print();
    }
};

class ASTDefArray : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* size_;
public:
    ASTDefArray(ASTNode* var, ASTNode* size) : var_(var), size_(size) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTDefArray() {}
  
    // Test
    virtual void print() {
        std::cout<<"ASTDefArray: \t"<<"def []"<<std::endl;
        var_->print();
        size_->print();
    }  
};

class ASTDefFunc : public ASTNode {
private:
    ASTNode* var_;
    std::vector<ASTNode*> parameters_;
    ASTNode* block_;
public:
    ASTDefFunc(ASTNode* var, const std::vector<ASTNode*>& parameters, ASTNode* block) : var_(var), parameters_(parameters), block_(block) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTDefFunc() {}
  
    // Test
    virtual void print() {
        std::cout<<"ASTDefFunc: \t"<<"def ("<<parameters_.size()<<")"<<std::endl;
        var_->print();
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->print();
        }
        block_->print();
    }  
};