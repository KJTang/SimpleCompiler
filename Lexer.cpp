#include "Lexer.h"

#include <cstdio>
#include <iostream>

Token Lexer::lexToken() {
    char curChar = lexingStr[lexingPos++];
    // shift space
    while (curChar == ' ') {
        curChar = lexingStr[lexingPos++];
    }
    // identifier
    // def: start with [a-zA-Z_], follow by [a-zA-Z_0-9]
    if (isalpha(curChar) || curChar == '_') {
        std::string identifierName;
        identifierName += curChar;
        curChar = lexingStr[lexingPos++];
        while (isalnum(curChar) || curChar == '_') {
            identifierName += curChar;
            curChar = lexingStr[lexingPos++];
        }
        --lexingPos;
        if (identifierName == "int") {
            tokens.push_back(std::make_pair(Token::KEYWORD_INT, identifierName));
            return Token::KEYWORD_INT;
        } else if (identifierName == "return") {
            tokens.push_back(std::make_pair(Token::KEYWORD_RET, identifierName));
            return Token::KEYWORD_RET;
        }
        tokens.push_back(std::make_pair(Token::IDENTIFIER, identifierName));
        return Token::IDENTIFIER;
    }
    // number
    // def: start with [0-9.], follow by [0-9.]
    // number only has 'double' type
    if (isdigit(curChar) || curChar == '.') {
        std::string numberStr;
        numberStr += curChar;
        curChar = lexingStr[lexingPos++];
        while (isdigit(curChar) || curChar == '.') {
            numberStr += curChar;
            curChar = lexingStr[lexingPos++];
        }
        --lexingPos;
        tokens.push_back(std::make_pair(Token::NUMBER, numberStr));
        return Token::NUMBER;
    }
    switch (curChar) {
        // semicolon
        // def: ';'
        case ';': {
            tokens.push_back(std::make_pair(Token::LABEL_SEMICOLON, ";"));
            return Token::LABEL_SEMICOLON;
        }
        // comma
        // def: ','
        case ',': {
            tokens.push_back(std::make_pair(Token::LABEL_COMMA, ","));
            return Token::LABEL_COMMA;
        }
        // paren
        // def: '(' or ')'
        case '(': {
            tokens.push_back(std::make_pair(Token::LABEL_L_PAREN, "("));
            return Token::LABEL_L_PAREN;
        }
        case ')': {
            tokens.push_back(std::make_pair(Token::LABEL_R_PAREN, ")"));
            return Token::LABEL_R_PAREN;
        }
        // brace
        // def: '{' or '}'
        case '{': {
            tokens.push_back(std::make_pair(Token::LABEL_L_BRACE, "{"));
            return Token::LABEL_L_BRACE;
        }
        case '}': {
            tokens.push_back(std::make_pair(Token::LABEL_R_BRACE, "}"));
            return Token::LABEL_R_BRACE;
        }
        // end of line
        case '\n': {
            tokens.push_back(std::make_pair(Token::END_OF_LINE, "\\n"));
            return Token::END_OF_LINE;
        }
        // end of file
        case '\0': {
            tokens.push_back(std::make_pair(Token::END_OF_FILE, "\0"));
            return Token::END_OF_FILE;
        }
        // comment
        // def: start with "//", will comment one line
        case '/': {
            if (lexingStr[lexingPos] == '/') {
                do {
                    curChar = lexingStr[lexingPos++];
                } while (curChar != '\0' && curChar != '\n');
                if (curChar != '\0') {
                    return this->lexToken();
                } else {
                    tokens.push_back(std::make_pair(Token::END_OF_FILE, "\0"));
                    return Token::END_OF_FILE;
                }
            }
            // if not '//', jump to default
        }
        // unknown
        default: {
            // std::cout<<"Lexing Unknown Character: "<<curChar<<std::endl;
            tokens.push_back(std::make_pair(Token::UNKNOWN, std::string(1, curChar)));
            return Token::UNKNOWN;
        }
    }
}

bool Lexer::lex() {
    Token result;
    do {
        result = this->lexToken();
    } while (result != Token::END_OF_FILE);

    if (result == Token::END_OF_FILE) {
        return true;
    } else {
        return false;
    }
}

void Lexer::print() {
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        std::cout<<"type: "<<(int)it->first<<";\tvalue: "<<it->second<<std::endl;
    }
}