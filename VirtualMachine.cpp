#include "VirtualMachine.h"

#include <sstream>
#include <cstring>

VirtualMachine::VirtualMachine(const std::string &code) {
    std::istringstream iss(code);
    int num = 0;
    while (iss>>num) {
        this->binCode.push_back(num);
    }
    int *global = new int[DATA_SEG_SIZE];
    this->data.push_back(global);
    bool *globalFlag = new bool[DATA_SEG_SIZE];
    memset(globalFlag, false, DATA_SEG_SIZE);
    this->dataFlag.push_back(globalFlag);
}

void VirtualMachine::printStack() {
    if (!this->stack.empty()) {
        std::cout<<"stack top: "<<this->stack.top()<<std::endl;
    } else {
        std::cout<<"stack top: none"<<std::endl;
    }
}

void VirtualMachine::printData() {
    for (int i = 0; i != eipStack.size()+1; ++i) {
        std::cout<<"data stack "<<i<<": ";
        for (int j = 0; j != 10; ++j) {
            std::cout<<data[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}

bool VirtualMachine::execute() {
    getNextWord();
    while (true) {
        switch (curWord) {
            case (int)OP::EXIT: {
                // std::cout<<"-- exit"<<std::endl;
                while (!this->data.empty()) {
                    delete this->data.back();
                    delete this->dataFlag.back();
                    this->data.pop_back();
                    this->dataFlag.pop_back();
                }
                return true;
            }
            case (int)OP::NOP: {
                // std::cout<<"-- nop"<<std::endl;
                getNextWord();
                break;
            }
            case (int)OP::PRIT: {
                // std::cout<<"-- prit"<<std::endl;
                std::cout<<this->stack.top()<<std::endl;
                this->stack.pop();
                getNextWord();
                break;
            }
            case (int)OP::SCAN: {
                getNextWord();
                // std::cout<<"-- scan "<<this->curWord<<std::endl;
                int pos = this->eipStack.size();
                while (pos != -1) {
                    if (this->dataFlag[pos][this->curWord]) {
                        std::cin>>this->data[pos][this->curWord];
                        getNextWord();
                        break;
                    } else {
                        --pos;
                    }
                }
                if (pos == -1) {
                    // std::cout<<"variable not found"<<std::endl;
                    return false;
                }
                break;
            }
            case (int)OP::ADD: {
                // std::cout<<"-- add"<<std::endl;
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(a+b);
                getNextWord();
                break;
            }
            case (int)OP::SUB: {
                // std::cout<<"-- sub"<<std::endl;
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b-a);
                getNextWord();
                break;
            }
            case (int)OP::MUL: {
                // std::cout<<"-- mul"<<std::endl;
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(a*b);
                getNextWord();
                break;
            }
            case (int)OP::DIV: {
                // std::cout<<"-- div"<<std::endl;
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b/a);
                getNextWord();
                break;
            }
            case (int)OP::GT: {
                // std::cout<<"-- gt"<<std::endl;
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b>a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::LT: {
                // std::cout<<"-- lt"<<std::endl;
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b<a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::DUP: {
                // std::cout<<"-- dup"<<std::endl;
                this->stack.push(this->stack.top());
                getNextWord();
                break;
            }
            case (int)OP::POP: {
                // std::cout<<"-- pop"<<std::endl;
                this->stack.pop();
                getNextWord();
                break;
            }
            case (int)OP::PUSH: {
                getNextWord();
                // std::cout<<"-- push "<<curWord<<std::endl;
                this->stack.push(curWord);
                getNextWord();
                break;
            }
            case (int)OP::JZ: {
                getNextWord();
                // std::cout<<"-- jz "<<this->curWord<<" "<<binCode[curWord]<<std::endl;
                if (this->stack.top() == 0) {
                    this->eip = curWord;
                }
                this->stack.pop();
                getNextWord();
                break;
            }
            case (int)OP::JMP: {
                getNextWord();
                // std::cout<<"-- jmp "<<this->curWord<<std::endl;
                this->eip = curWord;
                getNextWord();
                break;
            }
            case (int)OP::INT: {
                getNextWord();
                // std::cout<<"-- int "<<this->curWord<<std::endl;
                this->dataFlag[this->eipStack.size()][this->curWord] = true;
                getNextWord();
                break;
            }
            case (int)OP::LOAD: {
                getNextWord();
                // std::cout<<"-- load "<<this->curWord<<std::endl;
                int pos = this->eipStack.size();
                while (pos != -1) {
                    if (this->dataFlag[pos][this->curWord]) {
                        this->stack.push(this->data[pos][this->curWord]);
                        getNextWord();
                        break;
                    } else {
                        --pos;
                    }    
                }
                if (pos == -1) {
                    // std::cout<<"variable "<<this->curWord<<" not found"<<std::endl;
                    return false;
                }
                break;
            }
            case (int)OP::SAVE: {
                getNextWord();
                // std::cout<<"-- save "<<this->curWord<<std::endl;
                int pos = this->eipStack.size();
                while (pos != -1) {
                    if (this->dataFlag[pos][this->curWord]) {
                        this->data[pos][this->curWord] = this->stack.top();
                        this->stack.pop();
                        getNextWord();
                        break;
                    } else {
                        --pos;
                    }
                }
                if (pos == -1) {
                    // std::cout<<"variable not found"<<std::endl;
                    return false;
                }
                break;
            }
            case (int)OP::CALL: {
                getNextWord();
                // std::cout<<"-- call "<<this->curWord<<std::endl;
                this->eipStack.push(this->eip);
                this->eip = this->curWord;
                int *local = new int[DATA_SEG_SIZE];
                this->data.push_back(local);
                bool *localFlag = new bool[DATA_SEG_SIZE];
                memset(localFlag, false, DATA_SEG_SIZE);
                this->dataFlag.push_back(localFlag);
                getNextWord();
                break;
            }
            case (int)OP::FUNC: {
                getNextWord();
                // std::cout<<"-- func "<<this->curWord<<std::endl;
                while (this->curWord != (int)OP::ENDF) {
                    getNextWord();
                }
                break;
            }
            case (int)OP::RET: {
                // std::cout<<"-- ret "<<this->eipStack.top()<<std::endl;
                this->eip = this->eipStack.top();
                this->eipStack.pop();
                getNextWord();
                delete this->data.back();
                delete this->dataFlag.back();
                this->data.pop_back();
                this->dataFlag.pop_back();
                break;
            }
            case (int)OP::ENDF: {
                // std::cout<<"-- endf"<<std::endl;
                getNextWord();
                break;
            }
            default: {
                // std::cout<<"-- unknown: "<<this->curWord<<std::endl;
                return false;
            }
        }
        // this->printStack();
        // this->printData();
    }
    return true;
}
