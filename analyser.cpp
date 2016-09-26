#include "analyser.h"

void Analyser::Input(const std::vector<ASTNode*>& input_list) {
    input_list_ = input_list;
}

void Analyser::Output(std::vector<ASTNode*>& output_list) {
    output_list = output_list_;
}

bool Analyser::Analysis() {
    return true;
}