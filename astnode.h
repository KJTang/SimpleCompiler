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

    // false: this node can be remove
    virtual bool Analysis() {
        return true;
    }

    // Test
    virtual void Print() {}
};

class ASTBlock : public ASTNode {
private:
    std::vector<ASTNode*> statements_;
public:
    ASTBlock(const std::vector<ASTNode*>& statments) : statements_(statments) {
        set_type(ASTTYPE::DEFAULT);
    }
    ~ASTBlock() {
        for (int i = 0; i != statements_.size(); ++i) {
            delete statements_[i];
        }
    }

    virtual bool Analysis() {
        for (int i = 0; i < statements_.size(); ) {
            if (!statements_[i]->Analysis()) {
                delete statements_[i];
                statements_.erase(statements_.begin() + i);
            } else {
                ++i;
            }
        }
        if (statements_.size() == 0) {
            return false;
        } else {
            return true;
        }
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTBlock: \t"<<"{"<<statements_.size()<<"}"<<std::endl;
        for (int i = 0; i != statements_.size(); ++i) {
            statements_[i]->Print();
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
    virtual void Print() {
        std::cout<<"ASTConst: \t"<<get_value()<<std::endl;
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
    virtual void Print() {
        std::cout<<"ASTVariable: \t"<<get_value()<<std::endl;
    }
};

class ASTCallArray : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* expr_;
public:
    ASTCallArray(ASTNode* var, ASTNode* expr) : var_(var), expr_(expr) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTCallArray() {
        delete var_;
        delete expr_;
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTCallArray: \t"<<"[]"<<std::endl;
        var_->Print();
        expr_->Print();
    }
};

class ASTCallMember : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* member_;
public:
    ASTCallMember(ASTNode* var, ASTNode* member) : var_(var), member_(member) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTCallMember() {
        delete var_;
        delete member_;
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTCallMember: \t"<<"."<<std::endl;
        var_->Print();
        member_->Print();
    }
};

class ASTCallFunc : public ASTNode {
private:
    ASTNode* var_;
    std::vector<ASTNode*> parameters_;
public:
    ASTCallFunc(ASTNode* var, const std::vector<ASTNode*>& parameters) : var_(var), parameters_(parameters) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTCallFunc() {
        delete var_;
        for (int i = 0; i != parameters_.size(); ++i) {
            delete parameters_[i];
        }
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTCallFunc: \t"<<"("<<parameters_.size()<<")"<<std::endl;
        var_->Print();
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->Print();
        }
    }
};

class ASTOperatorBinary : public ASTNode {
private:
    int op_;
    ASTNode* l_node_;
    ASTNode* r_node_;
public:
    ASTOperatorBinary(int op, ASTNode* l_node, ASTNode* r_node) : op_(op), l_node_(l_node), r_node_(r_node) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTOperatorBinary() {
        delete l_node_;
        delete r_node_;
    }

    virtual bool Analysis() {
        l_node_->Analysis();
        r_node_->Analysis();
        return true;
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTOperatorBinary: \t"<<(char)op_<<"("<<op_<<")"<<std::endl;
        l_node_->Print();
        r_node_->Print();
    }
};

class ASTOperatorNew : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* class_name_;
    ASTNode* func_;
public:
    ASTOperatorNew(ASTNode* var, ASTNode* class_name, ASTNode* func) : var_(var), class_name_(class_name), func_(func) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTOperatorNew() {
        if (var_) {
            delete var_;
        }
        delete class_name_;
        delete func_;
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTOperatorNew: \t"<<"new class"<<std::endl;
        if (var_) {
            var_->Print();
        } else {
            std::cout<<"ASTOperatorNew: \tAnonymous"<<std::endl;
        }
        class_name_->Print();
        func_->Print();
    }
};

class ASTOperatorReturn : public ASTNode {
private:
    ASTNode* return_value_;
public:
    ASTOperatorReturn(ASTNode* return_value) : return_value_(return_value) {
        set_type(ASTTYPE::DEFAULT);
    }
    ~ASTOperatorReturn() {
        if (return_value_) {
            delete return_value_;
        }
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTOperatorReturn: \t"<<"return"<<std::endl;
        if (return_value_) {
            return_value_->Print();
        } else {
            std::cout<<"ASTOperatorReturn: \treturn null"<<std::endl;
        }
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
    ~ASTDefArray() {
        if (var_) {
            delete var_;
        }
        delete size_;
    }
  
    // Test
    virtual void Print() {
        std::cout<<"ASTDefArray: \t"<<"def []"<<std::endl;
        if (var_) {
            var_->Print();
        } else {
            std::cout<<"ASTDefArray: \tAnonymous"<<std::endl;
        }
        size_->Print();
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
    ~ASTDefFunc() {
        if (var_) {
            delete var_;
        }
        for (int i = 0; i != parameters_.size(); ++i) {
            delete parameters_[i];
        }
        delete block_;
    }
  
    // Test
    virtual void Print() {
        std::cout<<"ASTDefFunc: \t"<<"def ("<<parameters_.size()<<")"<<std::endl;
        if (var_) {
            var_->Print();
        } else {
            std::cout<<"ASTDefFunc: \tAnonymous"<<std::endl;
        }
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->Print();
        }
        block_->Print();
    }
};

class ASTDefClass : public ASTNode {
private:
    ASTNode* self_;
    ASTNode* parent_;
    ASTNode* block_;
public:
    ASTDefClass(ASTNode* self, ASTNode* parent, ASTNode* block) : self_(self), parent_(parent), block_(block) {
        set_type(ASTTYPE::DEFAULT);
    }
    ~ASTDefClass() {
        delete self_;
        if (parent_) {
            delete parent_;
        }
        delete block_;
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTDefClass: \t"<<"def class"<<std::endl;
        self_->Print();
        if (parent_) {
            parent_->Print();
        }
        block_->Print();
    }
};

class ASTExprAssign : public ASTNode {
private:
    ASTNode* l_node_;
    ASTNode* r_node_;
public:
    ASTExprAssign(ASTNode* l_node, ASTNode* r_node) : l_node_(l_node), r_node_(r_node) {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTExprAssign() {
        delete l_node_;
        delete r_node_;
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTExprAssign: \t"<<"="<<std::endl;
        l_node_->Print();
        r_node_->Print();
    }
};

class ASTExprIf : public ASTNode {
private:
    ASTNode* condition_;
    ASTNode* if_block_;
    ASTNode* else_block_;
public:
    ASTExprIf(ASTNode* condition, ASTNode* if_block, ASTNode* else_block) : condition_(condition), if_block_(if_block), else_block_(else_block) {
        set_type(ASTTYPE::DEFAULT);
    }
    ~ASTExprIf() {
        delete condition_;
        delete if_block_;
        if (else_block_) {
            delete else_block_;
        }
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTExprIf: \t"<<"if"<<std::endl;
        condition_->Print();
        if_block_->Print();
        if (else_block_) {
            else_block_->Print();
        }
    }  
};

class ASTExprWhile : public ASTNode {
private:
    ASTNode* condition_;
    ASTNode* block_;
public:
    ASTExprWhile(ASTNode* condition, ASTNode* block) : condition_(condition), block_(block) {
        set_type(ASTTYPE::DEFAULT);
    }
    ~ASTExprWhile() {
        delete condition_;
        delete block_;
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTExprWhile: \t"<<"while"<<std::endl;
        condition_->Print();
        block_->Print();
    }  
};