#include "Assembler.h"

bool Assembler::assemble() {
    this->readCode();
    return true;
}

bool Assembler::readCode() {
    std::istringstream iss(asmcode);
    std::string curToken;
    while (iss>>curToken) {
        auto it = this->opList.find(curToken);
        if (it == opList.end()) {
            // Invalid Operator
            return false;
        }
        Instruction newIns;
        newIns.opName = curToken;   // TODO: only used for test here, delete this later
        newIns.op = (*it).second.first;
        std::string arg;
        for (int i = 0; i != (*it).second.second; i++) {
            iss>>arg;    
            newIns.args.push_back(arg);
        }
        this->code.push_back(newIns);
    }
    return true;
}