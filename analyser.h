#pragma once

#include <vector>
#include <map>
#include <string>

#include "astnode.h"

static std::map<ASTTYPE, std::string> type_name = {
    {ASTTYPE::TYPE_NULL, "null"}, 
    {ASTTYPE::TYPE_BOOL, "bool"}, 
    {ASTTYPE::TYPE_INT, "int"}, 
    {ASTTYPE::TYPE_DOUBLE, "double"}, 
    {ASTTYPE::TYPE_STRING, "string"}, 
};

class Analyser {
private:
    std::vector<ASTNode*> ast_list_;
    int level = 0;

    bool err_occur_ = false;

    bool AnalysisNode(ASTNode* node);
    bool AnalysisBlock(ASTNode* block);
    bool AnalysisConst(ASTNode* con);
    bool AnalysisVariable(ASTNode* var);
    bool AnalysisCallArray(ASTNode* arr);
    bool AnalysisCallMember(ASTNode* mem);
    bool AnalysisCallFunc(ASTNode* func);
    bool AnalysisOperatorBinary(ASTNode* op);
    bool AnalysisOperatorNew(ASTNode* op);
    bool AnalysisOperatorReturn(ASTNode* op);
    bool AnalysisDecVar(ASTNode* dec);
    bool AnalysisDefArray(ASTNode* def);
    bool AnalysisDefFunc(ASTNode* def);
    bool AnalysisDefClass(ASTNode* def);
    bool AnalysisExprAssign(ASTNode* epxr);
    bool AnalysisExprIf(ASTNode* epxr);
    bool AnalysisExprWhile(ASTNode* epxr);
public:
    Analyser() {}
    ~Analyser() {}
    
    void Input(const std::vector<ASTNode*>& input_list);
    void Output(std::vector<ASTNode*>& output_list);
    bool Analysis();
};