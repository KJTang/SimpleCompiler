#pragma once

#include <string>
#include <vector>

enum class Token : int {
    IDENTIFIER, 
    NUMBER, 
    OPERATOR,           // operator: +-*/(){},;
    KEYWORD_IF,         // if
    KEYWORD_ELSE,       // else
    KEYWORD_WHILE,      // while
    KEYWORD_RET,        // return
    TYPE_INT,           // int
    // END_OF_LINE, 
    END_OF_FILE = -1
};

class Lexer {
private:
    std::string lexingStr;
    int lexingPos = 0;
    std::vector<std::pair<Token, std::string>> tokens;

    Token lexToken();
public:
    Lexer(const std::string &str, int pos = 0) : lexingStr(str), lexingPos(pos) {}
    ~Lexer() {}
    
    bool lex();
    void print();
    std::vector<std::pair<Token, std::string>> getTokens() {
        return tokens;
    }
};