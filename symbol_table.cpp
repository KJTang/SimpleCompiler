#include "symbol_table.h"

SymbolTable* SymbolTable::shared_ptr_ = nullptr;

bool SymbolTable::Insert(const std::string& name, SymbolType type, ASTNode* astnode) {
    SymbolElement* elem = Find(name);
    if (elem) {
        return false;
    }
    elem = new SymbolElement();
    elem->name = name;
    elem->type = type;
    elem->astnode = astnode;
    table_.insert(std::make_pair(name, elem));
    return true;
}

bool SymbolTable::Delete(const std::string& name) {
    SymbolElement* elem = Find(name);
    if (!elem) {
        return false;
    }
    delete elem;
    table_.erase(name);
    return true;
}

SymbolElement* SymbolTable::Find(const std::string& name) {
    auto result = table_.find(name);
    if (result == table_.end()) {
        return nullptr;
    } else {
        return result->second;
    }
}

bool SymbolTable::IsExist(const std::string& name) {
    return Find(name) == nullptr ? false : true;
}

void SymbolTable::Output() {
    std::cout<<"======================== SymbolTable ===================="<<std::endl;
    for (auto it = table_.begin(); it != table_.end(); ++it) {
        std::cout<<"Name: "<<it->first<<"\tValue: "<<it->second->astnode->get_value()<<std::endl;
    }
    std::cout<<"======================== SymbolTable ===================="<<std::endl;
}