#include "lexer.h"

Lexer::Lexer() {}

Lexer::~Lexer() {}

void Lexer::Input(const std::string& raw_str) {
    raw_str_ = raw_str;
}

void Lexer::Output(std::vector<std::pair<Token, std::string>>& tokens) {
    tokens = tokens_;

    // Test
    for (int i = 0; i != tokens_.size(); ++i) {
        if ((int)tokens_[i].first <= 255 && (int)tokens_[i].first >= 0) {
            std::cout<<tokens_[i].second<<std::endl;
        } else {
            std::cout<<(int)tokens_[i].first<<":\t"<<tokens_[i].second<<std::endl;
        }
    }
}

bool Lexer::Lex() {
    Token result;
    do {
        result = this->LexToken();
    } while (result != Token::END_OF_FILE);
    return true;
}

Token Lexer::LexToken() {
    cur_char_ = raw_str_[pos_++];
    // shift space and '\n'
    while (cur_char_ == ' ' || cur_char_ == '\n') {
        cur_char_ = raw_str_[pos_++];
    }
    // identifier
    // def: start with [a-zA-Z_], follow by [a-zA-Z_0-9]
    if (isalpha(cur_char_) || cur_char_ == '_') {
        return LexIdentifier();
    }

    // const int/double
    if (isdigit(cur_char_)) {
        return LexNumber();
    }
    // const string
    if (cur_char_ == '\"') {
        return LexString();
    }

    switch (cur_char_) {
        // end of file
        case '\0': {
            tokens_.push_back(std::make_pair(Token::END_OF_FILE, "\0"));
            return Token::END_OF_FILE;
        }
        case '>': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(std::make_pair(Token::OP_GTE, ">="));
                ++pos_;
                return Token::OP_GTE;
            }
            goto ASCII;
        }
        case '<': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(std::make_pair(Token::OP_LTE, "<="));
                ++pos_;
                return Token::OP_LTE;
            }
            goto ASCII;
        }
        case '=': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(std::make_pair(Token::OP_EQU, "=="));
                ++pos_;
                return Token::OP_EQU;
            }
            goto ASCII;
        }
        case '!': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(std::make_pair(Token::OP_NE, "!="));
                ++pos_;
                return Token::OP_NE;
            }
            goto ASCII;
        }
        case '&': {
            if (raw_str_[pos_] == '&') {
                tokens_.push_back(std::make_pair(Token::OP_AND, "&&"));
                ++pos_;
                return Token::OP_AND;
            }
            goto ASCII;
        }
        case '|': {
            if (raw_str_[pos_] == '|') {
                tokens_.push_back(std::make_pair(Token::OP_OR, "||"));
                ++pos_;
                return Token::OP_OR;
            }
            goto ASCII;
        }
        // comment
        // def: start with "//", will comment one line
        case '/': {
            if (raw_str_[pos_] == '/') {
                do {
                    cur_char_ = raw_str_[pos_++];
                } while (cur_char_ != '\0' && cur_char_ != '\n');
                if (cur_char_ != '\0') {
                    return this->LexToken();
                } else {
                    tokens_.push_back(std::make_pair(Token::END_OF_FILE, "\0"));
                    return Token::END_OF_FILE;
                }
            }
            // if not '//', jump to ascii
            goto ASCII;
        }
        // ascii
        default: {
            ASCII:
            tokens_.push_back(std::make_pair((Token)cur_char_, std::string(1, cur_char_)));
            return (Token)cur_char_;
        }
    }
}

Token Lexer::LexIdentifier() {
    std::string identifierName;
    cur_char_ = raw_str_[pos_-1];

    identifierName += cur_char_;
    cur_char_ = raw_str_[pos_++];
    while (isalnum(cur_char_) || cur_char_ == '_') {
        identifierName += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    --pos_;
    // const null/bool
    if (identifierName == "null") {
        tokens_.push_back(std::make_pair(Token::CONST_NULL, identifierName));
        return Token::CONST_NULL;
    } else if (identifierName == "true") {
        tokens_.push_back(std::make_pair(Token::CONST_BOOL, identifierName));
        return Token::CONST_BOOL;
    } else if (identifierName == "false") {
        tokens_.push_back(std::make_pair(Token::CONST_BOOL, identifierName));
        return Token::CONST_BOOL;
    }
    // keywords
    // def: class | if | else | while | return
    else if (identifierName == "class") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_CLASS, identifierName));
        return Token::KEYWORD_CLASS;
    } else if (identifierName == "if") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_IF, identifierName));
        return Token::KEYWORD_IF;
    } else if (identifierName == "else") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_ELSE, identifierName));
        return Token::KEYWORD_ELSE;
    } else if (identifierName == "while") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_WHILE, identifierName));
        return Token::KEYWORD_WHILE;
    } else if (identifierName == "return") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_RET, identifierName));
        return Token::KEYWORD_RET;
    }
    tokens_.push_back(std::make_pair(Token::IDENTIFIER, identifierName));
    return Token::IDENTIFIER;
}

Token Lexer::LexNumber() {
    std::string constStr;
    cur_char_ = raw_str_[pos_-1];

    constStr += cur_char_;
    cur_char_ = raw_str_[pos_++];
    bool isInt = true;
    while (isdigit(cur_char_) || cur_char_ == '.') {
        if (cur_char_ == '.') {
            isInt = false;
        }
        constStr += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    --pos_;
    if (isInt) {
        tokens_.push_back(std::make_pair(Token::CONST_INT, constStr));
        return Token::CONST_INT;
    } else {
        tokens_.push_back(std::make_pair(Token::CONST_DOUBLE, constStr));
        return Token::CONST_DOUBLE;
    }
}

Token Lexer::LexString() {
    std::string constStr;
    cur_char_ = raw_str_[pos_-1];

    cur_char_ = raw_str_[pos_++];
    while (cur_char_ != '\"') {
        constStr += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    tokens_.push_back(std::make_pair(Token::CONST_STRING, constStr));
    return Token::CONST_STRING;
}