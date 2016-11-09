#include "symbol_table.h"

SymbolTable* SymbolTable::shared_ptr_ = nullptr;

bool SymbolTable::InsertInLevel(const std::string& name, int level = 0) {
    if (level > top_) {
        return false;
    }
    SymbolElement* elem = FindInLevel(name, level);
    if (elem) {
        return false;
    }
    elem = new SymbolElement();
    elem->name = name;

    table_list_[level]->insert(std::make_pair(name, elem));
    return true;
}

bool SymbolTable::DeleteInLevel(const std::string& name, int level = 0) {
    if (level > top_) {
        return false;
    }
    SymbolElement* elem = FindInLevel(name, level);
    if (!elem) {
        return false;
    }
    delete elem;

    table_list_[level]->erase(name);
    return true;
}

SymbolElement* SymbolTable::FindInLevel(const std::string& name, int level = 0) {
    if (level > top_) {
        return nullptr;
    }
    auto result = table_list_[level]->find(name);
    if (result == table_list_[level]->end()) {
        return nullptr;
    } else {
        return result->second;
    }
}

bool SymbolTable::IsExistInLevel(const std::string& name, int level = 0) {
    return FindInLevel(name, level) == nullptr ? false : true;
}

bool SymbolTable::Insert(const std::string& name) {
    return InsertInLevel(name, top_);
}

bool SymbolTable::Delete(const std::string& name) {
    int level = top_;
    while (level >= 0) {
        SymbolElement* elem = FindInLevel(name, level);
        if (elem) {
            delete elem;
            table_list_[level]->erase(name);
            return true;
        }
        --level;
    }
    return false;
}

SymbolElement* SymbolTable::Find(const std::string& name) {
    SymbolElement* result = FindInLevel(name, top_);
    if (result) {
        return result;
    }
    int index = top_;
    while (!result && index != 0) {
        --index;
        result = FindInLevel(name, index);
    }
    return result;
}

bool SymbolTable::IsExist(const std::string& name) {
    return Find(name) == nullptr ? false : true;
}

bool SymbolTable::PushTable() {
    std::map<std::string, SymbolElement*>* table = new std::map<std::string, SymbolElement*>();
    table_list_.push_back(table);
    ++top_;
    return true;
}

bool SymbolTable::PopTable() {
    std::map<std::string, SymbolElement*>* table = table_list_[table_list_.size() - 1];
    for (auto it = (*table).begin(); it != (*table).end(); ++it) {
        delete it->second;
    }
    delete table;
    table_list_.pop_back();
    --top_;
    return true;
}

void SymbolTable::Output() {
    std::cout<<"======================== SymbolTable ===================="<<std::endl;
    for (int i = 0; i != table_list_.size(); ++i) {
        std::cout<<"Level "<<i<<std::endl;
        for (auto it = table_list_[i]->begin(); it != table_list_[i]->end(); ++it) {
            std::cout<<"Name: "<<it->first<<std::endl;
        }
    }
    std::cout<<"======================== SymbolTable ===================="<<std::endl;
}