#include "Assembler.h"

bool Assembler::assemble() {
    this->init();
    this->redirect();
    this->interpret();
    return true;
}

bool Assembler::init() {
    std::istringstream iss(asmcode);
    std::string curToken;
    while (iss>>curToken) {
        auto it = this->opList.find(curToken);
        if (it == opList.end()) {
            // Label
            if (curToken[curToken.size()-1] == ':') {
                this->labelMap.insert(std::make_pair(curToken.substr(0, curToken.size()-1), line));
                continue;
            }
            // Invalid Operator
            std::cout<<"Invalid Operator: "<<curToken<<std::endl;
            return false;
        }
        Instruction newIns;
        newIns.opName = curToken;   // TODO: only used for test here, delete this later
        newIns.op = (*it).second.first;
        std::string arg;
        for (int i = 0; i != (*it).second.second; i++) {
            iss>>arg;    
            newIns.args.push_back(arg);
            ++line;
        }
        this->instructions.push_back(newIns);
        ++line;
        // function declaration
        if (newIns.op == OP::FUNC) {
            this->funcMap.insert(std::make_pair(newIns.args[0], line));
        }
        // variable declaration
        if (newIns.op == OP::INT) {
            this->varMap.insert(std::make_pair(newIns.args[0], varID++));
        }
    }
    this->instructions.push_back({"CALL", OP::CALL, std::vector<std::string> {"main"}});
    this->instructions.push_back({"EXIT", OP::EXIT, std::vector<std::string> {}});
    return true;
}

bool Assembler::redirect() {
    for (auto it = this->instructions.begin(); it != this->instructions.end(); ++it) {
        // label
        if (it->op == OP::JZ || it->op == OP::JMP) {
            auto lableIt = this->labelMap.find(it->args[0]);
            std::ostringstream num;
            num<<lableIt->second;
            it->args[0] = num.str();
        } 
        // function
        else if (it->op == OP::FUNC) {
            auto funcIt = this->funcMap.find(it->args[0]);
            std::ostringstream num;
            num<<funcIt->second;
            it->args[0] = num.str();
        } else if (it->op == OP::CALL) {
            auto funcIt = this->funcMap.find(it->args[0]);
            if (funcIt == this->funcMap.end()) {
                std::cout<<"cannot find function: "<<it->args[0]<<std::endl;
                return false;
            }
            std::ostringstream num;
            num<<funcIt->second;
            it->args[0] = num.str();
        } 
        // variable
        else if (it->op == OP::INT) {
            auto varIt = this->varMap.find(it->args[0]);
            std::ostringstream num;
            num<<varIt->second;
            it->args[0] = num.str();
        } else if (it->op == OP::SAVE || it->op == OP::LOAD || it->op == OP::SCAN) {
            auto varIt = this->varMap.find(it->args[0]);
            if (varIt == this->varMap.end()) {
                std::cout<<"cannot find variable: "<<it->args[0]<<std::endl;
                return false;
            }
            std::ostringstream num;
            num<<varIt->second;
            it->args[0] = num.str();
        }
    }
    return true;
}

bool Assembler::interpret() {
    std::ostringstream oss;
    for (auto it = this->instructions.begin(); it != this->instructions.end(); ++it) {
        oss<<static_cast<int>(it->op)<<" ";
        for (int i = 0; i != it->args.size(); ++i) {
            oss<<it->args[i]<<" ";
        }
    }
    this->binCode = oss.str();
    return true;
}