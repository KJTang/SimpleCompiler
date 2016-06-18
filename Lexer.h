#pragma once

#include <string>
#include <vector>

enum class Token : int {
    UNKNOWN, 
    END_OF_LINE, 
    END_OF_FILE, 
    IDENTIFIER, 
    NUMBER, 
    LABEL_COMMA,        // ,
    LABEL_SEMICOLON,    // ;
    LABEL_L_PAREN,      // () 
    LABEL_R_PAREN,  
    LABEL_L_BRACE,      // {}
    LABEL_R_BRACE, 
    KEYWORD_INT,        // int
    KEYWORD_RET,        // return
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