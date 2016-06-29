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
                Instruction label;
                label.opName = "LABEL";   // TODO: only used for test here, delete this later
                label.op = OP::LABEL;
                label.args.push_back(curToken.substr(0, curToken.size()-1));
                this->instructions.push_back(label);
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
        }
        this->instructions.push_back(newIns);
    }
    this->instructions.push_back({"CALL", OP::CALL, std::vector<std::string> {"main"}});
    this->instructions.push_back({"EXIT", OP::EXIT, std::vector<std::string> {}});
    return true;
}

bool Assembler::redirect() {
    return true;
}

bool Assembler::interpret() {
    return true;
}