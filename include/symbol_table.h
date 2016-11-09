#pragma once

#include <string>
#include <map>
#include <vector>

#include "astnode.h"

struct SymbolElement {
    std::string name;
};

class SymbolTable {
private:
    static SymbolTable* shared_ptr_;

    std::vector<std::map<std::string, SymbolElement*>*> table_list_;
    int top_ = -1;
public:
    SymbolTable() {}
    ~SymbolTable() {
        for (int i = 0; i != table_list_.size(); ++i) {
            delete table_list_[i];
        }
    }
    
    static SymbolTable* GetInstance() {
        if (!shared_ptr_) {
            shared_ptr_ = new SymbolTable();
            shared_ptr_->PushTable();
        }
        return shared_ptr_;
    }

    bool InsertInLevel(const std::string& name, int level);
    bool DeleteInLevel(const std::string& name, int level);
    SymbolElement* FindInLevel(const std::string& name, int level);
    bool IsExistInLevel(const std::string& name, int level);

    bool Insert(const std::string& name);
    bool Delete(const std::string& name);
    SymbolElement* Find(const std::string& name);
    bool IsExist(const std::string& name);

    bool PushTable();
    bool PopTable();

    void Output();
};