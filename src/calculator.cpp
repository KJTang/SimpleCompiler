#include "calculator.h"

#include "converter.h"
#include "lexer.h"
#include "astnode.h"

#define ERR(line, str) \
do { \
    err_occur_ = true; \
    ErrorHandler::GetInstance()->ThrowError(line, str); \
} while (false);

#define WARN(line, str) \
do { \
    ErrorHandler::GetInstance()->ThrowWarning(line, str); \
} while (false);

Calculator* Calculator::shared_ptr_ = nullptr;

ASTNode* Calculator::Calculate(int line, int op, ASTNode* l, ASTNode* r) {
    switch (op) {
        case '+': 
        case '-': 
        case '*': 
        case '/': {
            return ValueCalculate(line, op, l, r);
        }
        case static_cast<int>(Token::OP_AND):     // &&
        case static_cast<int>(Token::OP_OR): {    // ||
            return LogicCalculate(line, op, l, r);
        }
        case '>': 
        case '<': 
        case static_cast<int>(Token::OP_GTE):     // >=
        case static_cast<int>(Token::OP_LTE):     // <=
        case static_cast<int>(Token::OP_EQU):     // ==
        case static_cast<int>(Token::OP_NE): {    // !=
            return ConditionCalculate(line, op, l, r);
        }
        default: {
            return nullptr;
        }
    }
    return nullptr;
}

ASTNode* Calculator::ValueCalculate(int line, int op, ASTNode* l, ASTNode* r) {
    if (l->get_type() == ASTTYPE::TYPE_STRING || r->get_type() == ASTTYPE::TYPE_STRING) {
        return ValueCalculateString(line, op, l, r);
    }
    if (l->get_type() == ASTTYPE::TYPE_DOUBLE || r->get_type() == ASTTYPE::TYPE_DOUBLE) {
        return ValueCalculateDouble(line, op, l, r);
    }
    if (l->get_type() == ASTTYPE::TYPE_INT || r->get_type() == ASTTYPE::TYPE_INT) {
        return ValueCalculateInt(line, op, l, r);
    }
    return nullptr;
}

ASTNode* Calculator::ValueCalculateInt(int line, int op, ASTNode* l, ASTNode* r) {
    int l_num = 0;
    int r_num = 0;
    int result = 0;

    switch (l->get_type()) {
        case ASTTYPE::TYPE_INT: {
            l_num = Converter::GetInstance()->ConvertStringToInt(l->get_value());
            break;
        }
        default: {
            return nullptr;
        }
    }
    switch (r->get_type()) {
        case ASTTYPE::TYPE_INT: {
            r_num = Converter::GetInstance()->ConvertStringToInt(r->get_value());
            break;
        }
        default: {
            return nullptr;
        }
    }
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
                WARN(line, "division by zero");
                return nullptr;
            }
        }
    }
    ASTNode* node = new ASTConst(line, Token::CONST_INT, Converter::GetInstance()->ConvertIntToString(result));
    return node;
}

ASTNode* Calculator::ValueCalculateDouble(int line, int op, ASTNode* l, ASTNode* r) {
    double l_num = 0.0;
    double r_num = 0.0;
    double result = 0.0;

    switch (l->get_type()) {
        case ASTTYPE::TYPE_INT: {
            l_num = Converter::GetInstance()->ConvertStringToInt(l->get_value());
            break;
        }
        case ASTTYPE::TYPE_DOUBLE: {
            l_num = Converter::GetInstance()->ConvertStringToDouble(l->get_value());
            break;
        }
        default: {
            return nullptr;
        }
    }
    switch (r->get_type()) {
        case ASTTYPE::TYPE_INT: {
            r_num = Converter::GetInstance()->ConvertStringToInt(r->get_value());
            break;
        }
        case ASTTYPE::TYPE_DOUBLE: {
            r_num = Converter::GetInstance()->ConvertStringToDouble(r->get_value());
            break;
        }
        default: {
            return nullptr;
        }
    }
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
                WARN(line, "division by zero");
                return nullptr;
            }
        }
    }
    ASTNode* node = new ASTConst(line, Token::CONST_DOUBLE, Converter::GetInstance()->ConvertDoubleToString(result));
    return node;
}

ASTNode* Calculator::ValueCalculateString(int line, int op, ASTNode* l, ASTNode* r) {
    std::string l_str;
    std::string r_str;
    std::string result;

    l_str = l->get_value();
    r_str = r->get_value();

    if (op == '+') {
        result = l_str + r_str;
    } else {
        // ERR(line, "")
        return nullptr;
    }

    ASTNode* node = new ASTConst(line, Token::CONST_STRING, result);
    return node;
}

ASTNode* Calculator::LogicCalculate(int line, int op, ASTNode* l, ASTNode* r) {
    bool l_bool, r_bool, result;

    switch (l->get_type()) {
        case ASTTYPE::TYPE_NULL: {
            l_bool = false;
            break;
        }
        case ASTTYPE::TYPE_BOOL: {
            l_bool = l->get_value() == "true" ? true : false;
            break;
        }
        case ASTTYPE::TYPE_INT: 
        case ASTTYPE::TYPE_DOUBLE: {
            l_bool = l->get_value() == "0" ? false : true;
            break;
        }
        case ASTTYPE::TYPE_STRING: {
            l_bool = true;
            break;
        }
    }
    switch (r->get_type()) {
        case ASTTYPE::TYPE_NULL: {
            r_bool = false;
            break;
        }
        case ASTTYPE::TYPE_BOOL: {
            r_bool = r->get_value() == "true" ? true : false;
            break;
        }
        case ASTTYPE::TYPE_INT: 
        case ASTTYPE::TYPE_DOUBLE: {
            r_bool = r->get_value() == "0" ? false : true;
            break;
        }
        case ASTTYPE::TYPE_STRING: {
            r_bool = true;
            break;
        }
    }
    switch (op) {
        case static_cast<int>(Token::OP_AND): {
            result = l_bool && r_bool;
            break;
        }
        case static_cast<int>(Token::OP_OR): { 
            result = l_bool || r_bool;
            break;
        }
    }

    ASTNode* node = new ASTConst(line, Token::CONST_BOOL, result ? "true" : "false");
    return node;
}

ASTNode* Calculator::ConditionCalculate(int line, int op, ASTNode* l, ASTNode* r) {
    double l_num = 0.0;
    double r_num = 0.0;
    std::string result;

    switch (l->get_type()) {
        case ASTTYPE::TYPE_INT: {
            l_num = Converter::GetInstance()->ConvertStringToInt(l->get_value());
            break;
        }
        case ASTTYPE::TYPE_DOUBLE: {
            l_num = Converter::GetInstance()->ConvertStringToDouble(l->get_value());
            break;
        }
        default: {
            return nullptr;
        }
    }
    switch (r->get_type()) {
        case ASTTYPE::TYPE_INT: {
            r_num = Converter::GetInstance()->ConvertStringToInt(r->get_value());
            break;
        }
        case ASTTYPE::TYPE_DOUBLE: {
            r_num = Converter::GetInstance()->ConvertStringToDouble(r->get_value());
            break;
        }
        default: {
            return nullptr;
        }
    }
    switch (op) {
        case static_cast<int>(Token::OP_GTE): {
            result = l_num >= r_num ? "true" : "false";
            break;
        }
        case static_cast<int>(Token::OP_LTE): { 
            result = l_num <= r_num ? "true" : "false";
            break;
        }
        case static_cast<int>(Token::OP_EQU): {
            result = l_num == r_num ? "true" : "false";
            break;
        }
        case static_cast<int>(Token::OP_NE): { 
            result = l_num != r_num ? "true" : "false";
            break;
        }
    }

    ASTNode* node = new ASTConst(line, Token::CONST_BOOL, result);
    return node;
}