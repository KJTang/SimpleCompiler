#include "VirtualMachine.h"

#include <sstream>
#include <cstring>

// #define DEBUG

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
                #ifdef DEBUG
                std::cout<<"-- exit"<<std::endl;
                #endif
                while (!this->data.empty()) {
                    delete[] this->data.back();
                    delete[] this->dataFlag.back();
                    this->data.pop_back();
                    this->dataFlag.pop_back();
                }
                return true;
            }
            case (int)OP::NOP: {
                #ifdef DEBUG
                std::cout<<"-- nop"<<std::endl;
                #endif
                getNextWord();
                break;
            }
            case (int)OP::PRIT: {
                #ifdef DEBUG
                std::cout<<"-- prit"<<std::endl;
                #endif
                std::cout<<this->stack.top()<<std::endl;
                this->stack.pop();
                getNextWord();
                break;
            }
            case (int)OP::SCAN: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- scan "<<this->curWord<<std::endl;
                #endif
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
                    #ifdef DEBUG
                    std::cout<<"variable not found"<<std::endl;
                    #endif
                    return false;
                }
                break;
            }
            case (int)OP::ADD: {
                #ifdef DEBUG
                std::cout<<"-- add"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(a+b);
                getNextWord();
                break;
            }
            case (int)OP::SUB: {
                #ifdef DEBUG
                std::cout<<"-- sub"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b-a);
                getNextWord();
                break;
            }
            case (int)OP::MUL: {
                #ifdef DEBUG
                std::cout<<"-- mul"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(a*b);
                getNextWord();
                break;
            }
            case (int)OP::DIV: {
                #ifdef DEBUG
                std::cout<<"-- div"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b/a);
                getNextWord();
                break;
            }
            case (int)OP::GT: {
                #ifdef DEBUG
                std::cout<<"-- gt"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b>a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::LT: {
                #ifdef DEBUG
                std::cout<<"-- lt"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b<a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::GTE: {
                #ifdef DEBUG
                std::cout<<"-- gte"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b>=a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::LTE: {
                #ifdef DEBUG
                std::cout<<"-- lte"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b<=a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::EQU: {
                #ifdef DEBUG
                std::cout<<"-- gt"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b==a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::NE: {
                #ifdef DEBUG
                std::cout<<"-- lt"<<std::endl;
                #endif
                int a = this->stack.top();
                this->stack.pop();
                int b = this->stack.top();
                this->stack.pop();
                this->stack.push(b!=a?1:0);
                getNextWord();
                break;
            }
            case (int)OP::DUP: {
                #ifdef DEBUG
                std::cout<<"-- dup"<<std::endl;
                #endif
                this->stack.push(this->stack.top());
                getNextWord();
                break;
            }
            case (int)OP::POP: {
                #ifdef DEBUG
                std::cout<<"-- pop"<<std::endl;
                #endif
                this->stack.pop();
                getNextWord();
                break;
            }
            case (int)OP::PUSH: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- push "<<curWord<<std::endl;
                #endif
                this->stack.push(curWord);
                getNextWord();
                break;
            }
            case (int)OP::JZ: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- jz "<<this->curWord<<" "<<binCode[curWord]<<std::endl;
                #endif
                if (this->stack.top() == 0) {
                    this->eip = curWord;
                }
                this->stack.pop();
                getNextWord();
                break;
            }
            case (int)OP::JMP: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- jmp "<<this->curWord<<std::endl;
                #endif
                this->eip = curWord;
                getNextWord();
                break;
            }
            case (int)OP::INT: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- int "<<this->curWord<<std::endl;
                #endif
                this->dataFlag[this->eipStack.size()][this->curWord] = true;
                getNextWord();
                break;
            }
            case (int)OP::LOAD: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- load "<<this->curWord<<std::endl;
                #endif
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
                    #ifdef DEBUG
                    std::cout<<"variable "<<this->curWord<<" not found"<<std::endl;
                    #endif
                    return false;
                }
                break;
            }
            case (int)OP::SAVE: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- save "<<this->curWord<<std::endl;
                #endif
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
                    #ifdef DEBUG
                    std::cout<<"variable not found"<<std::endl;
                    #endif
                    return false;
                }
                break;
            }
            case (int)OP::CALL: {
                getNextWord();
                #ifdef DEBUG
                std::cout<<"-- call "<<this->curWord<<std::endl;
                #endif
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
                #ifdef DEBUG
                std::cout<<"-- func "<<this->curWord<<std::endl;
                #endif
                while (this->curWord != (int)OP::ENDF) {
                    getNextWord();
                }
                break;
            }
            case (int)OP::RET: {
                #ifdef DEBUG
                std::cout<<"-- ret "<<this->eipStack.top()<<std::endl;
                #endif
                this->eip = this->eipStack.top();
                this->eipStack.pop();
                getNextWord();
                delete[] this->data.back();
                delete[] this->dataFlag.back();
                this->data.pop_back();
                this->dataFlag.pop_back();
                break;
            }
            case (int)OP::ENDF: {
                #ifdef DEBUG
                std::cout<<"-- endf"<<std::endl;
                #endif
                getNextWord();
                break;
            }
            default: {
                #ifdef DEBUG
                std::cout<<"-- unknown: "<<this->curWord<<std::endl;
                #endif
                return false;
            }
        }
        #ifdef DEBUG
        this->printStack();
        this->printData();
        #endif
    }
    return true;
}
