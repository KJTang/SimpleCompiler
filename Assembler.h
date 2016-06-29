#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

// operators
enum class OP {
    /* system */
    EXIT,
    NOP,
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
    INT,    // create a int variable
    LOAD,
    SAVE,
    /* function */
    CALL,
    FUNC,   // define a function
    RET,    // "return" in function definition
    ENDF,   // end of function definition
};

struct Instruction {
    std::string opName; // TODO: only used for test here, delete this later
    OP op;
    std::vector<std::string> args;
};

class Assembler {
private:
    std::string asmcode;
    std::map<std::string, std::pair<OP, int>> opList;

    std::vector<Instruction> code;
public:
    Assembler(const std::string &code) : asmcode(code) {
        this->opList = {
            {"EXIT", {OP::EXIT, 0}}, 
            {"NOP", {OP::NOP, 0}}, 
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
            {"INT", {OP::INT, 1}},
            {"LOAD", {OP::LOAD, 1}},
            {"SAVE", {OP::SAVE, 1}},
            {"CALL", {OP::CALL, 1}},
            {"FUNC", {OP::FUNC, 1}},
            {"RET", {OP::RET, 0}},
            {"ENDF", {OP::ENDF, 0}},
        };
    }
    ~Assembler() {}
    
    bool assemble();
    bool readCode();

    void print() {
        for (int i = 0; i != code.size(); ++i) {
            std::cout<<code[i].opName;
            for (int j = 0; j != code[i].args.size(); ++j) {
                std::cout<<" "<<code[i].args[j];
            }
            std::cout<<std::endl;
        }
    }
};