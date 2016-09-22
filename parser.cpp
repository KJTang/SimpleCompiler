#include "parser.h"

#include <iostream>
#include <stack>

Parser::Parser() {}

Parser::~Parser() {}

void Parser::Input(const std::vector<std::pair<Token, std::string>>& tokens) {
    tokens_ = tokens;
}

void Parser::Output(std::vector<ASTNode*>& astnode_list) {
    astnode_list = astnode_list_;

    // Test
    for (int i = 0; i != astnode_list_.size(); ++i) {
        std::cout<<"--------------------------------------------"<<std::endl;
        astnode_list_[i]->print();
    }
}

bool Parser::Parse() {
    cur_token_ = tokens_[pos_++];
    while (pos_ < tokens_.size()) {
        ASTNode* node = ParseStatement();
        if (err_occur_) {
            return false;
        }
        if (node) {
            astnode_list_.push_back(node);
        }
    }
    return true;
}

ASTNode* Parser::ParseStatement() {
    switch (cur_token_.first) {
        case Token::KEYWORD_IF: {
            return ParseIfExpression();
        }
        case Token::KEYWORD_WHILE: {
            return ParseWhileExpression();
        }
        case static_cast<Token>(';'): {
            cur_token_ = tokens_[pos_++];
            return ParseStatement();
        }
        default: {
            return ParseExpression();
        }
    }
}

ASTNode* Parser::ParseIfExpression() {
    return nullptr;
}

ASTNode* Parser::ParseWhileExpression() {
    return nullptr;
}

ASTNode* Parser::ParseExpression() {
    // std::cout<<"ParseExpression: "<<cur_token_.second<<std::endl;
    std::stack<ASTNode*> var_stack;
    std::stack<int> op_stack;

    bool is_finished = false;
    bool is_token_var = false;

    while (!is_finished) {
        switch (cur_token_.first) {
            case static_cast<Token>('('): {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;
                var_stack.push(ParseParenExpression());
                break;
            }
            case Token::IDENTIFIER: {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;
                var_stack.push(ParseVariable());
                break;
            }
            case Token::CONST_NULL:
            case Token::CONST_BOOL:
            case Token::CONST_INT:
            case Token::CONST_DOUBLE:
            case Token::CONST_STRING: {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;
                var_stack.push(ParseConst());
                break;
            }
            default: {
                if (!is_token_var) {
                    is_finished = true;
                    break;
                }
                if (binary_operator_precedence[static_cast<int>(cur_token_.first)] == 0) {
                    // std::cout<<"ParseExpression: end operator: "<<static_cast<char>(cur_token_.first)<<std::endl;
                    is_finished = true;
                    break;
                }
                is_token_var = false;

                if (!op_stack.empty() && 
                    binary_operator_precedence[op_stack.top()] >= binary_operator_precedence[static_cast<int>(cur_token_.first)]) {
                    int op = op_stack.top();
                    op_stack.pop();
                    ASTNode* r_node = var_stack.top();
                    var_stack.pop();
                    ASTNode* l_node = var_stack.top();
                    var_stack.pop();
                    ASTNode* node = new ASTBinaryOp(op, l_node, r_node);
                    var_stack.push(node);
                }
                op_stack.push(static_cast<int>(cur_token_.first));
                cur_token_ = tokens_[pos_++];
                break;
            }
        }
    }

    while (!op_stack.empty()) {
        int op = op_stack.top();
        op_stack.pop();
        ASTNode* r_node = var_stack.top();
        var_stack.pop();
        ASTNode* l_node = var_stack.top();
        var_stack.pop();
        ASTBinaryOp* node = new ASTBinaryOp(op, l_node, r_node);
        var_stack.push(node);
    }
    if (!var_stack.empty()) {
        return var_stack.top();
    } else {
        // TODO: err
        return nullptr;
    }
}

ASTNode* Parser::ParseParenExpression() {
    cur_token_ = tokens_[pos_++];   // eat '('
    ASTNode* expr = ParseExpression();
    if (!expr) {
        // TODO: err
        return nullptr;
    }
    if (static_cast<int>(cur_token_.first) != ')') {
        // TODO: err
        return nullptr;
    }
    cur_token_ = tokens_[pos_++];   // eat ')'
    return expr;
}

ASTNode* Parser::ParseConst() {
    // std::cout<<"ParseConst: "<<tokens_[pos_-1].second<<std::endl;
    ASTNode* node = new ASTConst(tokens_[pos_-1].first, tokens_[pos_-1].second);
    cur_token_ = tokens_[pos_++];
    return node;
}

ASTNode* Parser::ParseVariable() {
    // std::cout<<"ParseVariable: "<<tokens_[pos_-1].second<<std::endl;

    ASTNode* node = new ASTVariable(tokens_[pos_-1].second);
    cur_token_ = tokens_[pos_++];
    bool is_finished = false;
    do {
        switch (cur_token_.first) {
            case static_cast<Token>('['): {
                ASTNode* var_arr = ParseVarArray(node);
                if (!var_arr) {
                    // TODO: err
                    err_occur_ = true;
                    return nullptr;
                }
                node = var_arr;
                break;
            }
            case static_cast<Token>('.'): {
                ASTNode* var_mem = ParseVarMember(node);
                if (!var_mem) {
                    // TODO: err
                    err_occur_ = true;
                    return nullptr;
                }
                node = var_mem;
                break;
            }
            case static_cast<Token>('('): {
                ASTNode* var_func = ParseVarFunc(node);
                if (!var_func) {
                    // TODO: err
                    err_occur_ = true;
                    return nullptr;
                }
                node = var_func;
                break;
            }
            default: {
                is_finished = true;
                break;
            }
        }
    } while (!is_finished);

    return node;
}

ASTNode* Parser::ParseVarArray(ASTNode* var) {
    cur_token_ = tokens_[pos_++];   // eat '['
    ASTNode* expr = ParseExpression();
    if (!expr) {
        // TODO:err
        return nullptr;
    }
    if (static_cast<int>(cur_token_.first) != ']') {
        // TODO: err
        return nullptr;
    }
    cur_token_ = tokens_[pos_++];   // eat ']'
    ASTNode* node = new ASTVarArray(var, expr);
    return node;
}

ASTNode* Parser::ParseVarMember(ASTNode* var) {
    cur_token_ = tokens_[pos_++];   // eat '.'
    ASTNode* member = ParseVariable();
    if (!member) {
        // TODO:err
        return nullptr;
    }
    ASTNode* node = new ASTVarMember(var, member);
    return node;
}

ASTNode* Parser::ParseVarFunc(ASTNode* var) {
    cur_token_ = tokens_[pos_++];   // eat '('
    std::vector<ASTNode*> parameters;
    if (static_cast<int>(cur_token_.first) == ')') {
        cur_token_ = tokens_[pos_++];   // eat ')'
    } else {
        bool is_finished = false;
        while (!is_finished) {
            ASTNode* param = ParseExpression();
            if (!param) {
                // TODO: err
                return nullptr;
            }
            parameters.push_back(param);

            if (static_cast<int>(cur_token_.first) == ',') {
                cur_token_ = tokens_[pos_++];   // eat ','
            } else if (static_cast<int>(cur_token_.first) == ')') {
                cur_token_ = tokens_[pos_++];   // eat ')'
                is_finished = true;
            } else {
                // TODO: err
                return nullptr;
            }
        }
    }
    ASTNode* node = new ASTVarFunc(var, parameters);
    return node;
}