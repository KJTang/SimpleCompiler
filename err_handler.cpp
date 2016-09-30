#include "err_handler.h"

#include <iostream>

#include "converter.h"

ErrorHandler* ErrorHandler::shared_ptr_ = nullptr;

void ErrorHandler::ThrowError(int line, const std::string& info) {
    info_ += "Line: \t" + Converter::GetInstance()->ConvertIntToString(line) + "\tError: " + info + "\n";
}

void ErrorHandler::ThrowWarning(int line, const std::string& info) {
    info_ += "Line: \t" + Converter::GetInstance()->ConvertIntToString(line) + "\tWarning: " + info + "\n";
}

void ErrorHandler::Output() {
    std::cout<<"======================== Error Info ====================="<<std::endl;
    if (info_.size() == 0) {
        std::cout<<"0 error; 0 warning"<<std::endl;
    } else {
        std::cout<<info_;
    }
    std::cout<<"======================== Error Info ====================="<<std::endl;
}