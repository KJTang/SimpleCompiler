#include "err_handler.h"

#include <iostream>

ErrorHandler* ErrorHandler::shared_ptr = nullptr;

void ErrorHandler::ThrowError(int line, const std::string& info) {
    std::cout<<"Line: "<<line<<"\tError: "<<info<<std::endl;
}