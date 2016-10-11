#pragma once

#include <string>
#include <map>
#include <vector>

#include "astnode.h"

enum class SymbolType {
    VAR, 
    CLASS,
};

struct SymbolElement {
    std::string name;
    // SymbolType type;
    // ASTNode* astnode;
};

class SymbolTable {
private:
    static SymbolTable* shared_ptr_;

    std::vector<std::map<std::string, SymbolElement*>*> table_list_;
    int top_ = -1;
public:
    SymbolTable() {}
    ~SymbolTable() {
        // TODO: release memory
    }
    
    static SymbolTable* GetInstance() {
        if (!shared_ptr_) {
            shared_ptr_ = new SymbolTable();
            shared_ptr_->PushTable();
        }
        return shared_ptr_;
    }

    // bool InsertInLevel(const std::string& name, SymbolType type, ASTNode* astnode, int level);
    bool InsertInLevel(const std::string& name, int level);
    bool DeleteInLevel(const std::string& name, int level);
    SymbolElement* FindInLevel(const std::string& name, int level);
    bool IsExistInLevel(const std::string& name, int level);

    // bool Insert(const std::string& name, SymbolType type, ASTNode* astnode);
    bool Insert(const std::string& name);
    bool Delete(const std::string& name);
    SymbolElement* Find(const std::string& name);
    bool IsExist(const std::string& name);

    bool PushTable();
    bool PopTable();

    void Output();
};