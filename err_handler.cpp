#include "err_handler.h"

#include <iostream>

ErrorHandler* ErrorHandler::shared_ptr = nullptr;

void ErrorHandler::ThrowError(const std::string& info) {
    std::cout<<"Error: "<<info<<std::endl;
}