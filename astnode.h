#pragma once

#include <string>

enum class ASTTYPE {
    DEFAULT,
    VARIABLE,
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

    void set_type(ASTTYPE type) {type_ = type;}
    ASTTYPE get_type() {return type_;}

    void set_value(const std::string& value) {value_ = value;}
    std::string get_value() {return value_;}
};


class ASTVariable : public ASTNode {
public:
    ASTVariable() {
        set_type(ASTTYPE::VARIABLE);
    }
    ~ASTVariable() {}
};