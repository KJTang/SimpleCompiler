#pragma once

#include <iostream>
#include <vector>

#include "err_handler.h"

enum class Token : int {
    IDENTIFIER = 256, 

    CONST_NULL,         // const null: null
    CONST_BOOL,         // const bool: true/false
    CONST_INT,          // const int: 123
    CONST_DOUBLE,       // const double: 1.23
    CONST_STRING,       // const string: "str"

    OP_GTE,             // >=
    OP_LTE,             // <=
    OP_EQU,             // ==
    OP_NE,              // !=
    OP_AND,             // &&
    OP_OR,              // ||

    KEYWORD_CLASS,      // class
    KEYWORD_NEW,        // new
    KEYWORD_IF,         // if
    KEYWORD_ELSE,       // else
    KEYWORD_WHILE,      // while
    KEYWORD_VAR,      // var
    KEYWORD_ARRAY,      // array
    KEYWORD_FUNCTION,   // funtion
    KEYWORD_TABLE,      // table
    KEYWORD_RET,        // return

    END_OF_FILE = -1
};

struct TokenStruct {
    Token type;
    std::string value;
    int line;

    TokenStruct(Token t, const std::string& v, int l) {
        type = t;
        value = v;
        line = l;
    }
};

class Lexer {
private:
    // input
    std::string raw_str_;
    // output
    std::vector<TokenStruct*> tokens_;

    char cur_char_;
    int pos_ = 0;
    int line_ = 1;
    bool err_occur_ = false;

    Token LexToken();

    Token LexIdentifier();
    Token LexNumber();
    Token LexString();
public:
    Lexer();
    ~Lexer();
    
    void Input(const std::string& raw_str);
    void Output(std::vector<TokenStruct*>& tokens);
    bool Lex();
};