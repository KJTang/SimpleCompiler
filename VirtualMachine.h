#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <stack>

#include "Assembler.h"

const int CODE_SEG_SIZE = 100;
const int DATA_SEG_SIZE = 100;

class VirtualMachine {
private:
    std::vector<int> binCode;
    std::stack<int> stack;
    std::vector<int*> data;
    std::vector<bool*> dataFlag;

    int eip = 0;
    std::stack<int> eipStack;
    int curWord;
    int getNextWord() {
        return curWord = binCode[eip++];
    }

public:
    VirtualMachine(const std::string &code);
    ~VirtualMachine() {}
    
    void printStack();
    void printData();
    bool execute();
};