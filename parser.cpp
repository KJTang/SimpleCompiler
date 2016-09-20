#include "parser.h"

Parser::Parser() {}

Parser::~Parser() {}

void Parser::Input(const std::vector<std::pair<Token, std::string>>& tokens) {
    tokens_ = tokens;
}

void Parser::Output(std::vector<ASTNode*>& astnode_list) {
    astnode_list = astnode_list_;
}

bool Parser::Parse() {
    return true;
}