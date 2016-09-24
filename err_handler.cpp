#include "err_handler.h"

#include <iostream>
#include <algorithm>

std::string TransIntToString(int i) {
    if (i == 0) {
        return "0";
    }

    std::string s;
    while (i != 0) {
        s += (i % 10) + '0';
        i /= 10;
    }

    std::reverse(s.begin(), s.end());
    return s;
}

ErrorHandler* ErrorHandler::shared_ptr_ = nullptr;

void ErrorHandler::ThrowError(int line, const std::string& info) {
    info_ += "Line: \t" + TransIntToString(line) + "\tError: " + info + "\n";
}

void ErrorHandler::ThrowWarning(int line, const std::string& info) {
    info_ += "Line: \t" + TransIntToString(line) + "\tWarning: " + info + "\n";
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