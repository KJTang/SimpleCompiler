#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <array>
#include <stack>
#include <map>

const int CODE_SEG_SIZE = 100;
const int DATA_SEG_SIZE = 100;

// operators
enum class OP {
    /* system */
    EXIT,
    NOP,
    STACK_TOP, // used for test, show the top value of stack
    PRIT,
    SCAN,
    /* compute */
    ADD,
    SUB,
    MUL,
    DIV,
    /* stack */
    DUP,
    POP,
    PUSH,
    /* jump */
    JLZ,
    JGZ,
    JZ,
    JL,
    JG,
    JE,
    JMP,
    /* memory */
    LOAD,
    SAVE,
    /* function */
    CALL,
    RET,
};

struct Instruction {
    std::string opName; // just used for test
    OP op;
    std::vector<std::string> args;
};

class VirtualMachine {
private:
    std::string asmStr;
    std::map<std::string, std::pair<OP, int>> opList;

    std::vector<Instruction> code;
    std::stack<std::array<int, DATA_SEG_SIZE>*> data;
    std::stack<int> stack;

public:
    VirtualMachine(const std::string &str): asmStr(str) {
        code.reserve(CODE_SEG_SIZE);

        auto global = new std::array<int, DATA_SEG_SIZE>();
        data.push(global);

        this->opList = {
            {"EXIT", {OP::EXIT, 0}}, 
            {"NOP", {OP::NOP, 0}}, 
            {"STACK_TOP", {OP::STACK_TOP, 0}}, 
            {"ADD", {OP::ADD, 0}}, 
            {"SUB", {OP::SUB, 0}}, 
            {"MUL", {OP::MUL, 0}}, 
            {"DIV", {OP::DIV, 0}}, 
            {"DUP", {OP::DUP, 0}}, 
            {"POP", {OP::POP, 0}}, 
            {"PUSH", {OP::PUSH, 1}},
            {"JLZ", {OP::JLZ, 1}}, 
            {"JGZ", {OP::JGZ, 1}}, 
            {"JZ", {OP::JZ, 1}}, 
            {"JL", {OP::JL, 1}}, 
            {"JG", {OP::JG, 1}}, 
            {"JE", {OP::JE, 1}}, 
            {"JMP", {OP::JMP, 1}}, 
            {"LOAD", {OP::LOAD, 1}},
            {"SAVE", {OP::SAVE, 1}},
            {"CALL", {OP::CALL, 1}},
            {"RET", {OP::RET, 0}},
        };
    }
    ~VirtualMachine() {}
    
    bool readCode();
    bool outputCode();
    bool execCode();
};