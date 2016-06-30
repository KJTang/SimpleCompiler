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
    GT, // greater than
    LT, // less than
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
    /* label */
    LABEL,
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
    std::string binCode;

    std::map<std::string, std::pair<OP, int>> opList;
    std::vector<Instruction> instructions;
    int line = 0;
    int varID = 0;
    std::map<std::string, int> labelMap;
    std::map<std::string, int> funcMap;
    std::map<std::string, int> varMap;

    bool init();
    bool redirect();    // redirect labels and functions
    bool interpret();
public:
    Assembler(const std::string &code) : asmcode(code) {
        this->opList = {
            {"EXIT", {OP::EXIT, 0}}, 
            {"NOP", {OP::NOP, 0}}, 
            {"ADD", {OP::ADD, 0}}, 
            {"SUB", {OP::SUB, 0}}, 
            {"MUL", {OP::MUL, 0}}, 
            {"DIV", {OP::DIV, 0}}, 
            {"GT", {OP::GT, 0}}, 
            {"LT", {OP::LT, 0}}, 
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
    std::string &getBinCode() {
        return binCode;
    }

    void print() {
        for (int i = 0; i != instructions.size(); ++i) {
            std::cout<<instructions[i].opName;
            for (int j = 0; j != instructions[i].args.size(); ++j) {
                std::cout<<" "<<instructions[i].args[j];
            }
            std::cout<<std::endl;
        }
        std::cout<<"---- redirect ----"<<std::endl;
        for (auto it = labelMap.begin(); it != labelMap.end(); ++it) {
            std::cout<<"Label: "<<it->first<<" "<<it->second<<std::endl;
        }
        for (auto it = funcMap.begin(); it != funcMap.end(); ++it) {
            std::cout<<"Func: "<<it->first<<" "<<it->second<<std::endl;
        }
        for (auto it = varMap.begin(); it != varMap.end(); ++it) {
            std::cout<<"Var: "<<it->first<<" "<<it->second<<std::endl;
        }
        std::cout<<"------------------"<<std::endl;
    }
};