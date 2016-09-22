#pragma once

#include <string>

class ErrorHandler {
private:
    static ErrorHandler* shared_ptr;
public:
    ErrorHandler() {}
    ~ErrorHandler() {}
    
    static ErrorHandler* GetInstance() {
        if (!shared_ptr) {
            shared_ptr = new ErrorHandler();
        }
        return shared_ptr;
    }

    void ThrowError(const std::string& info);
};