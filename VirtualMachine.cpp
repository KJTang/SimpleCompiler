#include "VirtualMachine.h"

bool VirtualMachine::readCode() {
    std::istringstream iss(asmStr);
    std::string curToken;
    while (iss>>curToken) {
        transform (curToken.begin(), curToken.end(), curToken.begin(), toupper);
        auto it = this->opList.find(curToken);
        if (it == opList.end()) {
            // Invalid Operator
            return false;
        }
        Instruction newIns;
        newIns.opName = curToken;
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

bool VirtualMachine::outputCode() {
    int i = 0;
    for (auto it = this->code.begin(); it != this->code.end(); ++it) {
        std::cout<<i++<<"\t"<<(*it).opName;
        for (auto arg = (*it).args.begin(); arg != (*it).args.end(); ++arg) {
            std::cout<<" "<<(*arg);
        }
        std::cout<<std::endl;
    }
    return true;
}

bool VirtualMachine::execCode() {
    auto curIns = this->code.begin();
    while (true) {
        std::cout<<curIns - this->code.begin()<<std::endl;
        switch (curIns->op) {
            case OP::EXIT: {
                return true;
            }
            case OP::NOP: {
                break;
            }
            case OP::STACK_TOP: {
                if (!this->stack.empty()) {
                    std::cout<<"Top: "<<this->stack.top()<<std::endl;
                } else {
                    std::cout<<"Top: stack is empty"<<std::endl;
                }
                break;
            }
            case OP::ADD: {
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(a+b);
                break;
            }
            case OP::SUB: {
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b-a);
                break;
            }
            case OP::MUL: {
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(a*b);
                break;
            }
            case OP::DIV: {
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b/a);
                break;
            }
            case OP::DUP: {
                this->stack.push(this->stack.top());
                break;
            }
            case OP::POP: {
                this->stack.pop();
                break;
            }
            case OP::PUSH: {
                this->stack.push(atoi(curIns->args[0].c_str()));
                break;
            }
            case OP::JLZ: {
                int a = this->stack.top();
                this->stack.pop();
                if (a < 0) {
                    int addr = atoi(curIns->args[0].c_str());
                    curIns = this->code.begin() + addr - 1;
                }
                break;
            }
            case OP::JGZ: {
                int a = this->stack.top();
                this->stack.pop();
                if (a > 0) {
                    int addr = atoi(curIns->args[0].c_str());
                    curIns = this->code.begin() + addr - 1;
                }
                break;
            }
            case OP::JZ: {
                int a = this->stack.top();
                this->stack.pop();
                if (a == 0) {
                    int addr = atoi(curIns->args[0].c_str());
                    curIns = this->code.begin() + addr - 1;
                }
                break;
            }
            case OP::JL: {
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                if (b < a) {
                    int addr = atoi(curIns->args[0].c_str());
                    curIns = this->code.begin() + addr - 1;
                }
                break;
            }
            case OP::JG: {
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                if (b > a) {
                    int addr = atoi(curIns->args[0].c_str());
                    curIns = this->code.begin() + addr - 1;
                }
                break;
            }
            case OP::JE: {
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                if (b == a) {
                    int addr = atoi(curIns->args[0].c_str());
                    curIns = this->code.begin() + addr - 1;
                }
                break;
            }
            case OP::JMP: {
                int addr = atoi(curIns->args[0].c_str());
                curIns = this->code.begin() + addr - 1;
                break;
            }
            case OP::LOAD: {
                std::cout<<"load"<<std::endl;
                int addr = atoi(curIns->args[0].c_str());
                this->stack.push((*this->data.top())[addr]);
                break;
            }
            case OP::SAVE: {
                std::cout<<"save"<<std::endl;
                int addr = atoi(curIns->args[0].c_str());
                (*this->data.top())[addr] = this->stack.top();
                this->stack.pop();
                break;
            }
            case OP::CALL: {
                std::cout<<"call"<<std::endl;
                auto newLocaldata = new std::array<int, DATA_SEG_SIZE>();
                this->data.push(newLocaldata);
                int oldAddr = curIns - this->code.begin() + 1;
                (*newLocaldata)[0] = oldAddr;
                int newAddr = atoi(curIns->args[0].c_str());
                curIns = this->code.begin() + newAddr - 1;
                break;
            }
            case OP::RET: {
                std::cout<<"ret"<<std::endl;
                int oldAddr = (*this->data.top())[0];
                curIns = this->code.begin() + oldAddr - 1;
                delete this->data.top();
                this->data.pop();
                break;
            }
            default: {
                // Invalid Operator
                std::cout<<"Invalid Operator: "<<curIns->opName<<std::endl;
                return false;
            }
        }
        ++curIns;
    }
    return true;
}