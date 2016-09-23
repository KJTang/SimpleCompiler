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
    KEYWORD_FUNCTION,   // funtion
    KEYWORD_RET,        // return

    END_OF_FILE = -1
};

// struct TokenStruct {
//     Token type;
//     std::string value;
//     int line;
// };

class Lexer {
private:
    // input
    std::string raw_str_;
    // output
    std::vector<std::pair<Token, std::string>> tokens_;

    char cur_char_;
    int pos_ = 0;
    bool err_occur_ = false;

    Token LexToken();

    Token LexIdentifier();
    Token LexNumber();
    Token LexString();
public:
    Lexer();
    ~Lexer();
    
    void Input(const std::string& raw_str);
    void Output(std::vector<std::pair<Token, std::string>>& tokens);
    bool Lex();
};