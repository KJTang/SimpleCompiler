#pragma once

class Analyser {
private:
    std::vector<ASTNode*> input_list_;
    std::vector<ASTNode*> output_list_;

    bool err_occur_ = false;
public:
    Analyser() {}
    ~Analyser() {}
    
    void Input(const std::vector<ASTNode*>& input_list);
    void Output(std::vector<ASTNode*>& output_list);
    bool Analysis();
};