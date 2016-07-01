#include "Lexer.h"

#include <cstdio>
#include <iostream>

Token Lexer::lexToken() {
    char curChar = lexingStr[lexingPos++];
    // shift space and '\n'
    while (curChar == ' ' || curChar == '\n') {
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
        if (identifierName == "if") {
            tokens.push_back(std::make_pair(Token::KEYWORD_IF, identifierName));
            return Token::KEYWORD_IF;
        } else if (identifierName == "else") {
            tokens.push_back(std::make_pair(Token::KEYWORD_ELSE, identifierName));
            return Token::KEYWORD_ELSE;
        } else if (identifierName == "while") {
            tokens.push_back(std::make_pair(Token::KEYWORD_WHILE, identifierName));
            return Token::KEYWORD_WHILE;
        } else if (identifierName == "int") {
            tokens.push_back(std::make_pair(Token::TYPE_INT, identifierName));
            return Token::TYPE_INT;
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
        // end of file
        case '\0': {
            tokens.push_back(std::make_pair(Token::END_OF_FILE, "\0"));
            return Token::END_OF_FILE;
        }
        case '>': {
            if (lexingStr[lexingPos] == '=') {
                tokens.push_back(std::make_pair(Token::OP_GTE, ">="));
                ++lexingPos;
                return Token::OP_GTE;
            }
            goto ASCII;
        }
        case '<': {
            if (lexingStr[lexingPos] == '=') {
                tokens.push_back(std::make_pair(Token::OP_LTE, "<="));
                ++lexingPos;
                return Token::OP_LTE;
            }
            goto ASCII;
        }
        case '=': {
            if (lexingStr[lexingPos] == '=') {
                tokens.push_back(std::make_pair(Token::OP_EQU, "=="));
                ++lexingPos;
                return Token::OP_EQU;
            }
            goto ASCII;
        }
        case '!': {
            if (lexingStr[lexingPos] == '=') {
                tokens.push_back(std::make_pair(Token::OP_NE, "!="));
                ++lexingPos;
                return Token::OP_NE;
            }
            goto ASCII;
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
            // if not '//', jump to ascii
            goto ASCII;
        }
        // ascii
        default: {
            ASCII:
            tokens.push_back(std::make_pair((Token)curChar, std::string(1, curChar)));
            return (Token)curChar;
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
