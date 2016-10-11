#pragma once

#include <string>
#include <vector>

#include "lexer.h"

enum class ASTTYPE {
    DEFAULT,
    BLOCK, 
    VARIABLE, 
    TYPE_NULL,
    TYPE_BOOL,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    CALL_ARR, 
    CALL_MEMBER, 
    CALL_FUNC,
    OP_BIN,
    OP_NEW,
    OP_RET,
    DEC_VAR, 
    DEF_ARR,
    DEF_FUNC,
    DEF_TABLE,
    DEF_CLASS,
    EXPR_ASSIGN,
    EXPR_IF,
    EXPR_WHILE, 
};

class ASTNode {
private:
    bool is_const_ = false;
    ASTTYPE type_ = ASTTYPE::DEFAULT;
    std::string value_ = "null";
    ASTNode* parent_ = nullptr;
    int line_ = 0;
public:
    ASTNode() {}
    ~ASTNode() {}

    void set_is_const(bool is_const) { is_const_ = is_const; }
    bool get_is_const() { return is_const_; }

    void set_type(ASTTYPE type) { type_ = type; }
    ASTTYPE get_type() { return type_; }

    void set_value(const std::string& value) { value_ = value; }
    std::string get_value() { return value_; }

    void set_parent(ASTNode* parent) { parent_ = parent; }
    ASTNode* get_parent() { return parent_; }

    void set_line(int line) { line_ = line; }
    int get_line() { return line_; }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {}

    friend class Analyser;
    // Test
    virtual void Print() {}
};

class ASTBlock : public ASTNode {
private:
    std::vector<ASTNode*> statements_;
public:
    ASTBlock(int line, const std::vector<ASTNode*>& statements) : statements_(statements) {
        set_line(line);
        set_type(ASTTYPE::BLOCK);
        for (int i = 0; i != statements_.size(); ++i) {
            statements_[i]->set_parent(this);
        }
    }
    ~ASTBlock() {
        for (int i = 0; i != statements_.size(); ++i) {
            delete statements_[i];
        }
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        for (int i = 0; i != statements_.size(); ++i) {
            if (statements_[i] == cur_child) {
                statements_[i] = new_child;
                delete cur_child;
                break;
            }
        }
    }

    friend class Analyser;
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
    ASTConst(int line, Token token, const std::string& value) {
        set_line(line);
        set_is_const(true);
        switch (token) {
            case Token::CONST_NULL: {
                set_type(ASTTYPE::TYPE_NULL);
                break;
            }
            case Token::CONST_BOOL: {
                set_type(ASTTYPE::TYPE_BOOL);
                break;
            }
            case Token::CONST_INT: {
                set_type(ASTTYPE::TYPE_INT);
                break;
            }
            case Token::CONST_DOUBLE: {
                set_type(ASTTYPE::TYPE_DOUBLE);
                break;
            }
            case Token::CONST_STRING: {
                set_type(ASTTYPE::TYPE_STRING);
                break;
            }
        }
        set_value(value);
    }
    ~ASTConst() {}

    friend class Analyser;
    // Test
    virtual void Print() {
        std::cout<<"ASTConst: \t"<<get_value()<<std::endl;
    }
};

class ASTVariable : public ASTNode {
public:
    ASTVariable(int line, const std::string& var_name) {
        set_line(line);
        set_type(ASTTYPE::VARIABLE);
        set_value(var_name);
    }
    ~ASTVariable() {}

    friend class Analyser;
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
    ASTCallArray(int line, ASTNode* var, ASTNode* expr) : var_(var), expr_(expr) {
        set_line(line);
        set_type(ASTTYPE::CALL_ARR);
        var_->set_parent(this);
        expr_->set_parent(this);
    }
    ~ASTCallArray() {
        delete var_;
        delete expr_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == var_) {
            var_ = new_child;
            delete cur_child;
        } else if (cur_child == expr_) {
            expr_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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
    ASTCallMember(int line, ASTNode* var, ASTNode* member) : var_(var), member_(member) {
        set_line(line);
        set_type(ASTTYPE::CALL_MEMBER);
        var_->set_parent(this);
        member_->set_parent(this);
    }
    ~ASTCallMember() {
        delete var_;
        delete member_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == var_) {
            var_ = new_child;
            delete cur_child;
        } else if (cur_child == member_) {
            member_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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
    ASTCallFunc(int line, ASTNode* var, const std::vector<ASTNode*>& parameters) : var_(var), parameters_(parameters) {
        set_line(line);
        set_type(ASTTYPE::CALL_FUNC);
        var_->set_parent(this);
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->set_parent(this);
        }
    }
    ~ASTCallFunc() {
        delete var_;
        for (int i = 0; i != parameters_.size(); ++i) {
            delete parameters_[i];
        }
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == var_) {
            var_ = new_child;
            delete cur_child;
        } else {
            for (int i = 0; i != parameters_.size(); ++i) {
                if (parameters_[i] == cur_child) {
                    parameters_[i] = new_child;
                    delete cur_child;
                    break;
                }
            }
        }
    }

    friend class Analyser;
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
    ASTOperatorBinary(int line, int op, ASTNode* l_node, ASTNode* r_node) : op_(op), l_node_(l_node), r_node_(r_node) {
        set_line(line);
        set_type(ASTTYPE::OP_BIN);
        l_node_->set_parent(this);
        r_node_->set_parent(this);
    }
    ~ASTOperatorBinary() {
        delete l_node_;
        delete r_node_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == l_node_) {
            l_node_ = new_child;
            delete cur_child;
        } else if (cur_child == r_node_) {
            r_node_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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
    ASTOperatorNew(int line, ASTNode* var, ASTNode* class_name, ASTNode* func) : var_(var), class_name_(class_name), func_(func) {
        set_line(line);
        set_type(ASTTYPE::OP_NEW);
        if (var_) {
            var_->set_parent(this);
        }
        class_name_->set_parent(this);
        func_->set_parent(this);
    }
    ~ASTOperatorNew() {
        if (var_) {
            delete var_;
        }
        delete class_name_;
        delete func_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == var_) {
            var_ = new_child;
            delete cur_child;
        } else if (cur_child == class_name_) {
            class_name_ = new_child;
            delete cur_child;
        } else if (cur_child == func_) {
            func_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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
    ASTOperatorReturn(int line, ASTNode* return_value) : return_value_(return_value) {
        set_line(line);
        set_type(ASTTYPE::OP_RET);
        if (return_value_) {
            return_value_->set_parent(this);
        }
    }
    ~ASTOperatorReturn() {
        if (return_value_) {
            delete return_value_;
        }
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == return_value_) {
            return_value_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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

class ASTDecVar : public ASTNode {
private:
    std::vector<ASTNode*> var_list_;
public:
    ASTDecVar(int line, std::vector<ASTNode*>& var_list) : var_list_(var_list) {
        set_line(line);
        set_type(ASTTYPE::DEC_VAR);
        for (int i = 0; i != var_list_.size(); ++i) {
            var_list_[i]->set_parent(this);
        }
    }
    ~ASTDecVar() {
        for (int i = 0; i != var_list_.size(); ++i) {
            delete var_list_[i];
        }
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        for (int i = 0; i != var_list_.size(); ++i) {
            if (cur_child == var_list_[i]) {
                var_list_[i] = new_child;
                delete cur_child;
                break;
            }
        }
    }

    friend class Analyser;
    // Test
    virtual void Print() {
        std::cout<<"ASTDecVar: \t"<<var_list_.size()<<std::endl;
        for (int i = 0; i != var_list_.size(); ++i) {
            var_list_[i]->Print();
        }
    }  
};

class ASTDefArray : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* size_;
public:
    ASTDefArray(int line, ASTNode* var, ASTNode* size) : var_(var), size_(size) {
        set_line(line);
        set_type(ASTTYPE::DEF_ARR);
        if (var_) {
            var_->set_parent(this);
        }
        size_->set_parent(this);
    }
    ~ASTDefArray() {
        if (var_) {
            delete var_;
        }
        delete size_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == var_) {
            var_ = new_child;
            delete cur_child;
        } else if (cur_child == size_) {
            size_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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
    ASTNode* var_ = nullptr;
    std::vector<ASTNode*> parameters_;
    ASTNode* block_;
public:
    ASTDefFunc(int line, ASTNode* var, const std::vector<ASTNode*>& parameters, ASTNode* block) : var_(var), parameters_(parameters), block_(block) {
        set_line(line);
        set_type(ASTTYPE::DEF_FUNC);
        if (var_) {
            var_->set_parent(this);
        }
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->set_parent(this);
        }
        block_->set_parent(this);
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

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == var_) {
            var_ = new_child;
            delete cur_child;
        } else if (cur_child == block_) {
            block_ = new_child;
            delete cur_child;
        } else {
            for (int i = 0; i != parameters_.size(); ++i) {
                if (cur_child == parameters_[i]) {
                    parameters_[i] = new_child;
                    delete cur_child;
                    break;
                }
            }
        }
    }

    friend class Analyser;
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

class ASTDefTable : public ASTNode {
private:
    ASTNode* var_ = nullptr;
public:
    ASTDefTable(int line, ASTNode* var) : var_(var) {
        set_line(line);
        set_type(ASTTYPE::DEF_TABLE);
        if (var_) {
            var_->set_parent(this);
        }
    }
    ~ASTDefTable() {
        if (var_) {
            delete var_;
        }
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == var_) {
            var_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
    // Test
    virtual void Print() {
        std::cout<<"ASTDefTable: \t"<<"def table"<<std::endl;
        if (var_) {
            var_->Print();
        } else {
            std::cout<<"ASTDefTable: \tAnonymous"<<std::endl;
        }
    }
};

class ASTDefClass : public ASTNode {
private:
    ASTNode* self_;
    ASTNode* base_;
    ASTNode* block_;
public:
    ASTDefClass(int line, ASTNode* self, ASTNode* base, ASTNode* block) : self_(self), base_(base), block_(block) {
        set_line(line);
        set_type(ASTTYPE::DEF_CLASS);
        self_->set_parent(this);
        if (base_) {
            base_->set_parent(this);
        }
        block_->set_parent(this);
    }
    ~ASTDefClass() {
        delete self_;
        if (base_) {
            delete base_;
        }
        delete block_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == self_) {
            self_ = new_child;
            delete cur_child;
        } else if (cur_child == base_) {
            base_ = new_child;
            delete cur_child;
        } else if (cur_child == block_) {
            block_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
    // Test
    virtual void Print() {
        std::cout<<"ASTDefClass: \t"<<"def class"<<std::endl;
        self_->Print();
        if (base_) {
            base_->Print();
        }
        block_->Print();
    }
};

class ASTExprAssign : public ASTNode {
private:
    ASTNode* l_node_;
    ASTNode* r_node_;
public:
    ASTExprAssign(int line, ASTNode* l_node, ASTNode* r_node) : l_node_(l_node), r_node_(r_node) {
        set_line(line);
        set_type(ASTTYPE::EXPR_ASSIGN);
        l_node_->set_parent(this);
        r_node_->set_parent(this);
    }
    ~ASTExprAssign() {
        delete l_node_;
        delete r_node_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == l_node_) {
            l_node_ = new_child;
            delete cur_child;
        } else if (cur_child == r_node_) {
            r_node_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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
    ASTExprIf(int line, ASTNode* condition, ASTNode* if_block, ASTNode* else_block) : condition_(condition), if_block_(if_block), else_block_(else_block) {
        set_line(line);
        set_type(ASTTYPE::EXPR_IF);
        condition_->set_parent(this);
        if_block_->set_parent(this);
        if (else_block_) {
            else_block_->set_parent(this);
        }
    }
    ~ASTExprIf() {
        delete condition_;
        delete if_block_;
        if (else_block_) {
            delete else_block_;
        }
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == condition_) {
            condition_ = new_child;
            delete cur_child;
        } else if (cur_child == if_block_) {
            if_block_ = new_child;
            delete cur_child;
        } else if (cur_child == else_block_) {
            else_block_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
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
    ASTExprWhile(int line, ASTNode* condition, ASTNode* block) : condition_(condition), block_(block) {
        set_line(line);
        set_type(ASTTYPE::EXPR_WHILE);
        condition_->set_parent(this);
        block_->set_parent(this);
    }
    ~ASTExprWhile() {
        delete condition_;
        delete block_;
    }

    virtual void ReplaceChild(ASTNode* cur_child, ASTNode* new_child) {
        if (cur_child == condition_) {
            condition_ = new_child;
            delete cur_child;
        } else if (cur_child == block_) {
            block_ = new_child;
            delete cur_child;
        }
    }

    friend class Analyser;
    // Test
    virtual void Print() {
        std::cout<<"ASTExprWhile: \t"<<"while"<<std::endl;
        condition_->Print();
        block_->Print();
    }  
};