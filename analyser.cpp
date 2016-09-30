#include "analyser.h"

#include "err_handler.h"

#define ERR(line, str) \
do { \
    err_occur_ = true; \
    ErrorHandler::GetInstance()->ThrowError(line, str); \
} while (false);

#define WARN(line, str) \
do { \
    ErrorHandler::GetInstance()->ThrowWarning(line, str); \
} while (false);


void Analyser::Input(const std::vector<ASTNode*>& input_list) {
    input_list_ = input_list;
}

void Analyser::Output(std::vector<ASTNode*>& output_list) {
    output_list = output_list_;
}

bool Analyser::Analysis() {
    for (int i = 0; i != input_list_.size(); ++i) {
        AnalysisNode(input_list_[i]);
    }
    return true;
}

bool Analyser::AnalysisNode(ASTNode* node) {
    // std::cout<<"node: "<<(int)node->get_type()<<std::endl;
    switch (node->get_type()) {
        case ASTTYPE::BLOCK: {
            std::cout<<"Analysis: BLOCK"<<std::endl;
            AnalysisBlock(node);
            break;
        }
        case ASTTYPE::VARIABLE: {
            AnalysisVariable(node);
            std::cout<<"Analysis: VARIABLE"<<std::endl;
            break;
        }
        case ASTTYPE::TYPE_NULL: {
            std::cout<<"Analysis: TYPE_NULL"<<std::endl;
            AnalysisConst(node);
            break;
        }
        case ASTTYPE::TYPE_BOOL: {
            std::cout<<"Analysis: TYPE_BOOL"<<std::endl;
            AnalysisConst(node);
            break;
        }
        case ASTTYPE::TYPE_INT: {
            std::cout<<"Analysis: TYPE_INT"<<std::endl;
            AnalysisConst(node);
            break;
        }
        case ASTTYPE::TYPE_DOUBLE: {
            std::cout<<"Analysis: TYPE_DOUBLE"<<std::endl;
            AnalysisConst(node);
            break;
        }
        case ASTTYPE::TYPE_STRING: {
            std::cout<<"Analysis: TYPE_STRING"<<std::endl;
            AnalysisConst(node);
            break;
        }
        case ASTTYPE::CALL_ARR: {
            std::cout<<"Analysis: CALL_ARR"<<std::endl;
            AnalysisCallArray(node);
            break;
        }
        case ASTTYPE::CALL_MEMBER: {
            std::cout<<"Analysis: CALL_MEMBER"<<std::endl;
            AnalysisCallMember(node);
            break;
        }
        case ASTTYPE::CALL_FUNC: {
            std::cout<<"Analysis: CALL_FUNC"<<std::endl;
            AnalysisCallFunc(node);
            break;
        }
        case ASTTYPE::OP_BIN: {
            std::cout<<"Analysis: OP_BIN "<<(char)(((ASTOperatorBinary*)node)->op_)<<std::endl;
            AnalysisOperatorBinary(node);
            break;
        }
        case ASTTYPE::OP_NEW: {
            std::cout<<"Analysis: OP_NEW"<<std::endl;
            AnalysisOperatorNew(node);
            break;
        }
        case ASTTYPE::OP_RET: {
            std::cout<<"Analysis: OP_RET"<<std::endl;
            AnalysisOperatorReturn(node);
            break;
        }
        case ASTTYPE::DEF_ARR: {
            std::cout<<"Analysis: DEF_ARR"<<std::endl;
            AnalysisDefArray(node);
            break;
        }
        case ASTTYPE::DEF_FUNC: {
            std::cout<<"Analysis: DEF_FUNC"<<std::endl;
            AnalysisDefFunc(node);
            break;
        }
        case ASTTYPE::DEF_CLASS: {
            std::cout<<"Analysis: DEF_CLASS"<<std::endl;
            AnalysisDefClass(node);
            break;
        }
        case ASTTYPE::EXPR_ASSIGN: {
            std::cout<<"Analysis: EXPR_ASSIGN"<<std::endl;
            AnalysisExprAssign(node);
            break;
        }
        case ASTTYPE::EXPR_IF: {
            std::cout<<"Analysis: EXPR_IF"<<std::endl;
            AnalysisExprIf(node);
            break;
        }
        case ASTTYPE::EXPR_WHILE: {
            std::cout<<"Analysis: EXPR_WHILE"<<std::endl;
            AnalysisExprWhile(node);
            break;
        }
        default: {
            std::cout<<"========================================"<<std::endl;
            break;
        }
    }
}

bool Analyser::AnalysisBlock(ASTNode* block) {
    ASTBlock* block_ = static_cast<ASTBlock*>(block);
    for (int i = 0; i != block_->statements_.size(); ++i) {
        AnalysisNode(block_->statements_[i]);
    }
    return true;
}

bool Analyser::AnalysisConst(ASTNode* con) {
    ASTConst* const_ = static_cast<ASTConst*>(con);
    return true;
}

bool Analyser::AnalysisVariable(ASTNode* var) {
    ASTVariable* var_ = static_cast<ASTVariable*>(var);
    return true;
}

bool Analyser::AnalysisCallArray(ASTNode* arr) {
    ASTCallArray* arr_ = static_cast<ASTCallArray*>(arr);
    AnalysisNode(arr_->var_);
    AnalysisNode(arr_->expr_);
    return true;
}

bool Analyser::AnalysisCallMember(ASTNode* mem) {
    ASTCallMember* mem_ = static_cast<ASTCallMember*>(mem);
    AnalysisNode(mem_->var_);
    AnalysisNode(mem_->member_);
    return true;
}

bool Analyser::AnalysisCallFunc(ASTNode* func) {
    ASTCallFunc* func_ = static_cast<ASTCallFunc*>(func);
    AnalysisNode(func_->var_);
    for (int i = 0; i != func_->parameters_.size(); ++i) {
        AnalysisNode(func_->parameters_[i]);
    }
    return true;
}

bool Analyser::AnalysisOperatorBinary(ASTNode* op) {
    ASTOperatorBinary* op_ = static_cast<ASTOperatorBinary*>(op);
    AnalysisNode(op_->l_node_);
    AnalysisNode(op_->r_node_);
    if (op_->l_node_->get_is_const() && op_->r_node_->get_is_const()) {
        switch (op_->l_node_->get_type()) {
            case ASTTYPE::TYPE_NULL: {
                // TODO: 
                ERR(op_->get_line(), "type '" + type_name[op_->r_node_->get_type()] + "' cannot compute with type 'null'");
                break;
            }
            case ASTTYPE::TYPE_BOOL: {
                // TODO: 
                ERR(op_->get_line(), "type '" + type_name[op_->r_node_->get_type()] + "' cannot compute with type 'bool'");
                break;
            }
            case ASTTYPE::TYPE_INT: {
                switch (op_->r_node_->get_type()) {
                    case ASTTYPE::TYPE_INT: {
                        //
                        break;
                    }
                }
                break;
            }
        }
    }
    return true;
}

bool Analyser::AnalysisOperatorNew(ASTNode* op) {
    ASTOperatorNew* op_ = static_cast<ASTOperatorNew*>(op);
    if (op_->var_) {
        AnalysisNode(op_->var_);
    }
    AnalysisNode(op_->class_name_);
    AnalysisNode(op_->func_);
    return true;
}

bool Analyser::AnalysisOperatorReturn(ASTNode* op) {
    ASTOperatorReturn* op_ = static_cast<ASTOperatorReturn*>(op);
    if (op_->return_value_) {
        AnalysisNode(op_->return_value_);
    }
    return true;
}

bool Analyser::AnalysisDefArray(ASTNode* def) {
    ASTDefArray* def_ = static_cast<ASTDefArray*>(def);
    if (def_->var_) {
        AnalysisNode(def_->var_);
    }
    AnalysisNode(def_->size_);
    return true;
}

bool Analyser::AnalysisDefFunc(ASTNode* def) {
    ASTDefFunc* def_ = static_cast<ASTDefFunc*>(def);
    if (def_->var_) {
        AnalysisNode(def_->var_);
    }
    for (int i = 0; i != def_->parameters_.size(); ++i) {
        AnalysisNode(def_->parameters_[i]);
    }
    AnalysisNode(def_->block_);
    return true;
}

bool Analyser::AnalysisDefClass(ASTNode* def) {
    ASTDefClass* def_ = static_cast<ASTDefClass*>(def);
    AnalysisNode(def_->self_);
    if (def_->base_) {
        AnalysisNode(def_->base_);
    }
    AnalysisNode(def_->block_);
    return true;
}

bool Analyser::AnalysisExprAssign(ASTNode* expr) {
    ASTExprAssign* expr_ = static_cast<ASTExprAssign*>(expr);
    AnalysisNode(expr_->l_node_);
    AnalysisNode(expr_->r_node_);
    return true;
}

bool Analyser::AnalysisExprIf(ASTNode* expr) {
    ASTExprIf* expr_ = static_cast<ASTExprIf*>(expr);
    AnalysisNode(expr_->condition_);
    AnalysisNode(expr_->if_block_);
    if (expr_->else_block_) {
        AnalysisNode(expr_->else_block_);
    }
    return true;
}

bool Analyser::AnalysisExprWhile(ASTNode* expr) {
    ASTExprWhile* expr_ = static_cast<ASTExprWhile*>(expr);
    AnalysisNode(expr_->condition_);
    AnalysisNode(expr_->block_);
    return true;
}