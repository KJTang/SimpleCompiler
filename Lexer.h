#pragma once

#include <iostream>
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
    std::vector<std::pair<Token, std::string>> getTokens() {
        return tokens;
    }

    void debug() {
        std::cout<<"-------- Lexer --------"<<std::endl;
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            std::cout<<"type: "<<(int)it->first<<";\tvalue: "<<it->second<<std::endl;
        }
        std::cout<<"-----------------------"<<std::endl;
    }
};