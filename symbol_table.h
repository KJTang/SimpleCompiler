#pragma once

#include <string>
#include <map>

#include "astnode.h"

enum class SymbolType {
    VAR, 
    CLASS,
};

struct SymbolElement {
    std::string name;
    SymbolType type;
    ASTNode* astnode;
};

class SymbolTable {
private:
    static SymbolTable* shared_ptr_;

    std::map<std::string, SymbolElement*> table_;
public:
    SymbolTable() {}
    ~SymbolTable() {
        // TODO: release memory
    }
    
    static SymbolTable* GetInstance() {
        if (!shared_ptr_) {
            shared_ptr_ = new SymbolTable();
        }
        return shared_ptr_;
    }

    bool Insert(const std::string& name, SymbolType type, ASTNode* astnode);
    bool Delete(const std::string& name);
    SymbolElement* Find(const std::string& name);
    bool IsExist(const std::string& name);

    void Output();
};