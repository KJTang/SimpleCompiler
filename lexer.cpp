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

    if (err_occur_) {
        return false;
    } else {
        return true;
    }
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
    std::string identifier_name;
    cur_char_ = raw_str_[pos_-1];

    identifier_name += cur_char_;
    cur_char_ = raw_str_[pos_++];
    while (isalnum(cur_char_) || cur_char_ == '_') {
        identifier_name += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    --pos_;
    // const null/bool
    if (identifier_name == "null") {
        tokens_.push_back(std::make_pair(Token::CONST_NULL, identifier_name));
        return Token::CONST_NULL;
    } else if (identifier_name == "true") {
        tokens_.push_back(std::make_pair(Token::CONST_BOOL, identifier_name));
        return Token::CONST_BOOL;
    } else if (identifier_name == "false") {
        tokens_.push_back(std::make_pair(Token::CONST_BOOL, identifier_name));
        return Token::CONST_BOOL;
    }
    // keywords
    // def: class | if | else | while | return
    else if (identifier_name == "class") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_CLASS, identifier_name));
        return Token::KEYWORD_CLASS;
    } else if (identifier_name == "new") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_NEW, identifier_name));
        return Token::KEYWORD_NEW;
    } else if (identifier_name == "if") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_IF, identifier_name));
        return Token::KEYWORD_IF;
    } else if (identifier_name == "else") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_ELSE, identifier_name));
        return Token::KEYWORD_ELSE;
    } else if (identifier_name == "while") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_WHILE, identifier_name));
        return Token::KEYWORD_WHILE;
    } else if (identifier_name == "function") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_FUNCTION, identifier_name));
        return Token::KEYWORD_FUNCTION;
    } else if (identifier_name == "return") {
        tokens_.push_back(std::make_pair(Token::KEYWORD_RET, identifier_name));
        return Token::KEYWORD_RET;
    }
    tokens_.push_back(std::make_pair(Token::IDENTIFIER, identifier_name));
    return Token::IDENTIFIER;
}

Token Lexer::LexNumber() {
    std::string const_str;
    cur_char_ = raw_str_[pos_-1];

    const_str += cur_char_;
    cur_char_ = raw_str_[pos_++];
    bool is_int = true;
    while (isdigit(cur_char_) || cur_char_ == '.') {
        if (cur_char_ == '.') {
            is_int = false;
        }
        const_str += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    --pos_;
    if (is_int) {
        tokens_.push_back(std::make_pair(Token::CONST_INT, const_str));
        return Token::CONST_INT;
    } else {
        tokens_.push_back(std::make_pair(Token::CONST_DOUBLE, const_str));
        return Token::CONST_DOUBLE;
    }
}

Token Lexer::LexString() {
    std::string const_str;

    cur_char_ = raw_str_[pos_++];
    while (cur_char_ != '\"') {
        if (pos_ >= raw_str_.size()) {
            err_occur_ = true;
            ErrorHandler::GetInstance()->ThrowError("Missing \"");
            return Token::END_OF_FILE;
        }
        const_str += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    tokens_.push_back(std::make_pair(Token::CONST_STRING, const_str));
    return Token::CONST_STRING;
}