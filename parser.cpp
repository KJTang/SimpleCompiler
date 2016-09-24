#include "parser.h"

#include <iostream>
#include <stack>

#define ERR(line, str) \
do { \
    err_occur_ = true; \
    ErrorHandler::GetInstance()->ThrowError(line, str); \
} while (false);

#define WARN(line, str) \
do { \
    ErrorHandler::GetInstance()->ThrowWarning(line, str); \
} while (false);

Parser::Parser() {}

Parser::~Parser() {
    for (int i = 0; i != astnode_list_.size(); ++i) {
        delete astnode_list_[i];
    }
}

void Parser::Input(const std::vector<TokenStruct*>& tokens) {
    tokens_ = tokens;
}

void Parser::Output(std::vector<ASTNode*>& astnode_list) {
    astnode_list = astnode_list_;

    // Test
    std::cout<<"======================== Parse Start ===================="<<std::endl;
    for (int i = 0; i != astnode_list_.size(); ++i) {
        astnode_list_[i]->print();
    }
    std::cout<<"======================== Parse End ======================"<<std::endl;
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
    switch (cur_token_->type) {
        case Token::END_OF_FILE: {
            cur_token_ = tokens_[pos_++];
            return nullptr;
        }
        case Token::KEYWORD_IF: {
            return ParseIfExpression();
        }
        case Token::KEYWORD_WHILE: {
            return ParseWhileExpression();
        }
        case Token::KEYWORD_CLASS: {
            return ParseDefClass();
        }
        case Token::KEYWORD_RET: {
            return ParseReturnOperator();
        }
        case static_cast<Token>(';'): {
            cur_token_ = tokens_[pos_++];
            return ParseStatement();
        }
        case static_cast<Token>('}'): {
            if (block_count_ <= 0) {
                WARN(cur_token_->line, "unexpected token '}'");
                cur_token_ = tokens_[pos_++];   // eat '}'
                return ParseStatement();
            }
        }
        case static_cast<Token>('['): 
        case static_cast<Token>('('): 
        case Token::CONST_NULL: 
        case Token::CONST_BOOL: 
        case Token::CONST_INT: 
        case Token::CONST_DOUBLE: 
        case Token::CONST_STRING: 
        case Token::KEYWORD_NEW: 
        case Token::KEYWORD_FUNCTION: 
        case Token::IDENTIFIER: {
            return ParseExpression();
        }
        default: {
            ERR(cur_token_->line, "invalid expression");
            std::cout<<cur_token_->value<<std::endl;
            return nullptr;
        }
    }
}

ASTNode* Parser::ParseBlock() {
    // std::cout<<"ParseBlock: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat '{'
    ++block_count_;
    std::vector<ASTNode*> statements;

    if (static_cast<int>(cur_token_->type) == '}') {
        cur_token_ = tokens_[pos_++];   // eat '}'
        --block_count_;
    } else {
        bool is_finished = false;
        while (!is_finished) {
            ASTNode* stat = ParseStatement();
            // error occurs when parse statement
            if (err_occur_) {
                for (int i = 0; i != statements.size(); ++i) {
                    delete statements[i];
                }
                return nullptr;
            }
            if (stat) {
                statements.push_back(stat);
            }
            if (static_cast<int>(cur_token_->type) == '}') {
                is_finished = true;
            }
        }
        cur_token_ = tokens_[pos_++];   // eat '}'
        --block_count_;
    }

    ASTNode* node = new ASTBlock(statements);
    return node;
}

ASTNode* Parser::ParseIfExpression() {
    // std::cout<<"ParseIfExpression: "<<cur_token_->value<<std::endl;

    cur_token_= tokens_[pos_++];    // eat "if"
    ASTNode* condition = ParseParenExpression();
    // error occurs when parse paren expression
    if (err_occur_) {
        return nullptr;
    }
    
    ASTNode* if_block = nullptr;
    if (static_cast<int>(cur_token_->type) == '{') {
        if_block = ParseBlock();
    } else {
        if_block = ParseStatement();
    }
    // error occurs when parse block/statement
    if (err_occur_) {
        return nullptr;
    }

    ASTNode* else_block = nullptr;
    if (cur_token_->type == Token::KEYWORD_ELSE) {
        cur_token_ = tokens_[pos_++];   // eat "else"
        if (static_cast<int>(cur_token_->type) == '{') {
            else_block = ParseBlock();
        } else {
            else_block = ParseStatement();
        }
        // error occurs when parse block/statement
        if (err_occur_) {
            return nullptr;
        }
    }

    ASTNode* node = new ASTIfExpr(condition, if_block, else_block);
    return node;
}

ASTNode* Parser::ParseWhileExpression() {
    // std::cout<<"ParseWhileExpression: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat "while"
    ASTNode* condition = ParseParenExpression();
    // error occurs when parse paren expression
    if (err_occur_) {
        return nullptr;
    }

    ASTNode* block = nullptr;
    if (static_cast<int>(cur_token_->type) == '{') {
        block = ParseBlock();
    } else {
        block = ParseStatement();
    }
    // error occurs when parse block/statement
    if (err_occur_) {
        return nullptr;
    }

    ASTNode* node = new ASTWhileExpr(condition, block);
    return node;
}

ASTNode* Parser::ParseExpression() {
    // std::cout<<"ParseExpression: "<<cur_token_->value<<std::endl;

    std::stack<ASTNode*> var_stack;
    std::stack<int> op_stack;

    bool is_finished = false;
    bool is_token_var = false;

    while (!is_finished) {
        switch (cur_token_->type) {
            case static_cast<Token>('='): {
                if (!is_token_var) {
                    ERR(cur_token_->line, "expected primary-expression before '=' token");
                    while (!var_stack.empty()) {
                        delete var_stack.top();
                        var_stack.pop();
                    }
                    return nullptr;
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
                    return ParseAssignment(var_stack.top());
                } else {
                    ERR(cur_token_->line, "expression is invalid");
                    return nullptr;
                }
            }
            case static_cast<Token>('('): {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;

                ASTNode* expr = ParseParenExpression();
                // error occurs when parse paren expression 
                if (err_occur_) {
                    while (!var_stack.empty()) {
                        delete var_stack.top();
                        var_stack.pop();
                    }
                    return nullptr;
                }
                var_stack.push(expr);
                break;
            }
            // anoymous array
            case static_cast<Token>('['): {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;

                ASTNode* arr = ParseDefArray(nullptr);
                // error occurs when parse new array 
                if (err_occur_) {
                    while (!var_stack.empty()) {
                        delete var_stack.top();
                        var_stack.pop();
                    }
                    return nullptr;
                }
                var_stack.push(arr);
                break;
            }
            // anoymous function
            case Token::KEYWORD_FUNCTION: {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;

                ASTNode* func = ParseDefFunc(nullptr);
                // error occurs when parse function definition 
                if (err_occur_) {
                    while (!var_stack.empty()) {
                        delete var_stack.top();
                        var_stack.pop();
                    }
                    return nullptr;
                }
                var_stack.push(func);
                break;
            }
            // anoymous class instance
            case Token::KEYWORD_NEW: {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;

                ASTNode* new_class = ParseNewClassOperator(nullptr);
                // error occurs when parse new class 
                if (err_occur_) {
                    while (!var_stack.empty()) {
                        delete var_stack.top();
                        var_stack.pop();
                    }
                    return nullptr;
                }
                var_stack.push(new_class);
                break;
            }
            case Token::IDENTIFIER: {
                if (is_token_var) {
                    is_finished = true;
                    break;
                }
                is_token_var = true;

                ASTNode* var = ParseVariable();
                // error occurs when parse variable 
                if (err_occur_) {
                    while (!var_stack.empty()) {
                        delete var_stack.top();
                        var_stack.pop();
                    }
                    return nullptr;
                }
                var_stack.push(var);
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

                ASTNode* var = ParseConst();
                // error occurs when parse const 
                if (err_occur_) {
                    while (!var_stack.empty()) {
                        delete var_stack.top();
                        var_stack.pop();
                    }
                    return nullptr;
                }
                var_stack.push(var);
                break;
            }
            default: {
                if (!is_token_var) {
                    is_finished = true;
                    break;
                }
                if (binary_operator_precedence[static_cast<int>(cur_token_->type)] == 0) {
                    // std::cout<<"ParseExpression: end operator: "<<static_cast<char>(cur_token_->type)<<std::endl;
                    is_finished = true;
                    break;
                }
                is_token_var = false;

                if (!op_stack.empty() && 
                    binary_operator_precedence[op_stack.top()] >= binary_operator_precedence[static_cast<int>(cur_token_->type)]) {
                    int op = op_stack.top();
                    op_stack.pop();
                    ASTNode* r_node = var_stack.top();
                    var_stack.pop();
                    ASTNode* l_node = var_stack.top();
                    var_stack.pop();
                    ASTNode* node = new ASTBinaryOp(op, l_node, r_node);
                    var_stack.push(node);
                }
                op_stack.push(static_cast<int>(cur_token_->type));
                cur_token_ = tokens_[pos_++];
                break;
            }
        }
    }

    // empty expression
    if (op_stack.empty()) {
        if (var_stack.empty()) {
            return nullptr;
        } else {
            while (var_stack.size() != 1) {
                delete var_stack.top();
                var_stack.pop();
            }
            return var_stack.top();
        }
    }
    // not empty expression
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
        ERR(cur_token_->line, "expression is invalid");
        return nullptr;
    }
}

ASTNode* Parser::ParseParenExpression() {
    // std::cout<<"ParseParenExpression: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat '('
    if (static_cast<int>(cur_token_->type) == ')') {
        ERR(cur_token_->line, "expected expression between '(' and ')' token");
        return nullptr;
    }
    ASTNode* expr = ParseExpression();
    // error occurs when parse expression
    if (err_occur_) {
        return nullptr;
    }
    if (static_cast<int>(cur_token_->type) != ')') {
        ERR(cur_token_->line, "expression ')'");
        return nullptr;
    }
    cur_token_ = tokens_[pos_++];   // eat ')'
    return expr;
}

ASTNode* Parser::ParseConst() {
    // std::cout<<"ParseConst: "<<cur_token_->value<<std::endl;

    ASTNode* node = new ASTConst(tokens_[pos_-1]->type, tokens_[pos_-1]->value);
    cur_token_ = tokens_[pos_++];
    return node;
}

ASTNode* Parser::ParseVariable() {
    // std::cout<<"ParseVariable: "<<cur_token_->value<<std::endl;

    ASTNode* node = new ASTVariable(tokens_[pos_-1]->value);
    cur_token_ = tokens_[pos_++];
    bool is_finished = false;
    do {
        switch (cur_token_->type) {
            case static_cast<Token>('['): {
                ASTNode* var_arr = ParseCallArray(node);
                if (err_occur_) {
                    return nullptr;
                }
                node = var_arr;
                break;
            }
            case static_cast<Token>('.'): {
                ASTNode* var_mem = ParseCallMember(node);
                if (err_occur_) {
                    return nullptr;
                }
                node = var_mem;
                break;
            }
            case static_cast<Token>('('): {
                ASTNode* var_func = ParseCallFunc(node);
                if (err_occur_) {
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

ASTNode* Parser::ParseCallArray(ASTNode* var) {
    // std::cout<<"ParseCallArray: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat '['
    ASTNode* expr = ParseExpression();
    if (err_occur_) {
        return nullptr;
    }
    if (static_cast<int>(cur_token_->type) != ']') {
        ERR(cur_token_->line, "expected ']'");
        return nullptr;
    }
    cur_token_ = tokens_[pos_++];   // eat ']'
    ASTNode* node = new ASTCallArray(var, expr);
    return node;
}

ASTNode* Parser::ParseCallMember(ASTNode* var) {
    // std::cout<<"ParseCallMember: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat '.'
    ASTNode* member = ParseVariable();
    if (err_occur_) {
        return nullptr;
    }
    ASTNode* node = new ASTCallMember(var, member);
    return node;
}

ASTNode* Parser::ParseCallFunc(ASTNode* var) {
    // std::cout<<"ParseCallFunc: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat '('
    std::vector<ASTNode*> parameters;
    if (static_cast<int>(cur_token_->type) == ')') {
        cur_token_ = tokens_[pos_++];   // eat ')'
    } else {
        bool is_finished = false;
        while (!is_finished) {
            ASTNode* param = ParseExpression();
            if (err_occur_) {
                return nullptr;
            }
            // if function has parameters
            if (param) {
                parameters.push_back(param);
            }

            if (static_cast<int>(cur_token_->type) == ',') {
                cur_token_ = tokens_[pos_++];   // eat ','
            } else if (static_cast<int>(cur_token_->type) == ')') {
                cur_token_ = tokens_[pos_++];   // eat ')'
                is_finished = true;
            } else {
                ERR(cur_token_->line, "expected '')'");
                return nullptr;
            }
        }
    }
    ASTNode* node = new ASTCallFunc(var, parameters);
    return node;
}

ASTNode* Parser::ParseAssignment(ASTNode* var) {
    // std::cout<<"ParseAssignment: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat '='
    ASTNode* node = nullptr;
    if (static_cast<int>(cur_token_->type) == '[') {
        ASTNode* arr = ParseDefArray(var);
        if (err_occur_) {
            return nullptr;
        }
        node = arr;
    } else if (cur_token_->type == Token::KEYWORD_FUNCTION) {
        ASTNode* func = ParseDefFunc(var);
        if (err_occur_) {
            return nullptr;
        }
        node = func;
    } else if (cur_token_->type == Token::KEYWORD_NEW) {
        ASTNode* new_class = ParseNewClassOperator(var);
        if (err_occur_) {
            return nullptr;
        }
        node = new_class;
    } else {
        ASTNode* expr = ParseExpression();
        if (err_occur_) {
            return nullptr;
        }
        if (!expr) {
            ERR(cur_token_->line, "expected value after '=' token");
            return nullptr;
        }
        node = new ASTAssign(var, expr);
    }
    return node;
}

ASTNode* Parser::ParseDefArray(ASTNode* var) {
    // std::cout<<"ParseDefArray: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat '['
    ASTNode* size = ParseExpression();
    // error occurs when parse expression
    if (err_occur_) {
        return nullptr;
    }
    if (static_cast<int>(cur_token_->type) != ']') {
        ERR(cur_token_->line, "expected ']'");
        return nullptr;
    }
    cur_token_ = tokens_[pos_++];   // eat ']'
    ASTNode* node = new ASTDefArray(var, size);
    return node;
}

ASTNode* Parser::ParseDefFunc(ASTNode* var) {
    // std::cout<<"ParseDefFunc: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat "function"
    if (static_cast<int>(cur_token_->type) != '(') {
        ERR(cur_token_->line, "expected parameters after 'function' token");
        return nullptr;
    }
    
    cur_token_ = tokens_[pos_++];   // eat '('
    std::vector<ASTNode*> parameters;
    if (static_cast<int>(cur_token_->type) == ')') {
        cur_token_ = tokens_[pos_++];   // eat ')'
    } else {
        bool is_finished = false;
        while (!is_finished) {
            ASTNode* param = ParseExpression();
            if (err_occur_) {
                return nullptr;
            }
            // if function has parameters
            if (param) {
                parameters.push_back(param);
            }

            if (static_cast<int>(cur_token_->type) == ',') {
                cur_token_ = tokens_[pos_++];   // eat ','
            } else if (static_cast<int>(cur_token_->type) == ')') {
                cur_token_ = tokens_[pos_++];   // eat ')'
                is_finished = true;
            } else {
                ERR(cur_token_->line, "expected ')");
                return nullptr;
            }
        }
    }

    if (static_cast<int>(cur_token_->type) != '{') {
        ERR(cur_token_->line, "expected function definition");
        return nullptr;
    }
    ASTNode* block = ParseBlock();
    // error occurs when parse block
    if (err_occur_) {
        return nullptr;
    }
    ASTNode* node = new ASTDefFunc(var, parameters, block);
    return node;
}

ASTNode* Parser::ParseDefClass() {
    // std::cout<<"ParseDefClass: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat "class"
    if (cur_token_->type != Token::IDENTIFIER) {
        ERR(cur_token_->line, "expected class name after 'class' token");
        return nullptr;
    }
    
    ASTNode* self = new ASTVariable(cur_token_->value);
    cur_token_ = tokens_[pos_++];

    ASTNode* parent = nullptr;
    if (static_cast<int>(cur_token_->type) == ':') {
        cur_token_ = tokens_[pos_++];   // eat ':'
        if (cur_token_->type != Token::IDENTIFIER) {
            ERR(cur_token_->line, "expected parent class name after ':' token");
            return nullptr;
        }
        parent = new ASTVariable(cur_token_->value);
        cur_token_ = tokens_[pos_++];
    }

    ASTNode* block = nullptr;
    if (static_cast<int>(cur_token_->type) != '{') {
        ERR(cur_token_->line, "expected class definition");
        return nullptr;
    }
    block = ParseBlock();
    // error occurs when parse block
    if (err_occur_) {
        return nullptr;
    }

    ASTNode* node = new ASTDefClass(self, parent, block);
    return node;
}

ASTNode* Parser::ParseNewClassOperator(ASTNode* var) {
    // std::cout<<"ParseNewClassOperator: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat "new"
    if (cur_token_->type != Token::IDENTIFIER) {
        ERR(cur_token_->line, "expected type-specifier after 'new' token");
        return nullptr;
    }
    ASTNode* class_name = new ASTVariable(cur_token_->value);
    cur_token_ = tokens_[pos_++];
    if (static_cast<int>(cur_token_->type) != '(') {
        ERR(cur_token_->line, "expected '(' after type-specifier");
        return nullptr;
    }
    ASTNode* func = ParseCallFunc(class_name);
    // error occurs when parse function definition
    if (err_occur_) {
        return nullptr;
    }

    ASTNode* node = new ASTNewClassOp(var, class_name, func);
    return node;
}

ASTNode* Parser::ParseReturnOperator() {
    // std::cout<<"ParseReturnOperator: "<<cur_token_->value<<std::endl;

    cur_token_ = tokens_[pos_++];   // eat "return"
    ASTNode* expr = ParseExpression();
    if (err_occur_) {
        return nullptr;
    }
    ASTNode* node = new ASTReturnOp(expr);
    return node;
}
