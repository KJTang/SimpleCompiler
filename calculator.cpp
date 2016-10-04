#include "calculator.h"

#include "converter.h"
#include "lexer.h"
#include "astnode.h"

Calculator* Calculator::shared_ptr_ = nullptr;

ASTNode* Calculator::Calculate(int line, int op, ASTNode* l, ASTNode* r) {
    switch (op) {
        case '+': 
        case '-': 
        case '*': 
        case '/': {
            return ValueCalculate(line, op, l, r);
        }
        case static_cast<int>(Token::OP_GTE):     // >=
        case static_cast<int>(Token::OP_LTE):     // <=
        case static_cast<int>(Token::OP_EQU):     // ==
        case static_cast<int>(Token::OP_NE):      // !=
        case static_cast<int>(Token::OP_AND):     // &&
        case static_cast<int>(Token::OP_OR): {    // ||
            return LogicCalculate(line, op, l, r);
        }
        default: {
            return nullptr;
        }
    }
    return nullptr;
}

ASTNode* Calculator::ValueCalculate(int line, int op, ASTNode* l, ASTNode* r) {
    switch (l->get_type()) {
        case ASTTYPE::TYPE_INT: {
            return ValueCalculateInt(line, op, l, r);
        }
        default: {
            return nullptr;
        }
    }
    return nullptr;
}

ASTNode* Calculator::ValueCalculateInt(int line, int op, ASTNode* l, ASTNode* r) {
    switch (r->get_type()) {
        case ASTTYPE::TYPE_INT: {
            int l_num = Converter::GetInstance()->ConvertStringToInt(l->get_value());
            int r_num = Converter::GetInstance()->ConvertStringToInt(r->get_value());
            int result = 0;
            switch (op) {
                case '+': {
                    result = l_num + r_num;
                    break;
                }
                case '-': {
                    result = l_num - r_num;
                    break;
                }
                case '*': {
                    result = l_num * r_num;
                    break;
                }
                case '/': {
                    if (r_num != 0) {
                        result = l_num / r_num;
                        break;
                    } else {
                        return nullptr;
                    }
                }
                default: {
                    return nullptr;
                }
            }
            ASTNode* node = new ASTConst(line, Token::CONST_INT, Converter::GetInstance()->ConvertIntToString(result));
            return node;
        }
        case ASTTYPE::TYPE_STRING: {
            
            break;
        }
        default: {
            return nullptr;
        }
    }
    return nullptr;
}


ASTNode* Calculator::LogicCalculate(int line, int op, ASTNode* l, ASTNode* r) {
    return nullptr;
}