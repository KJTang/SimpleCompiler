#pragma once

#include <string>

class ErrorHandler {
private:
    static ErrorHandler* shared_ptr_;
    std::string info_;
public:
    ErrorHandler() {}
    ~ErrorHandler() {}
    
    static ErrorHandler* GetInstance() {
        if (!shared_ptr_) {
            shared_ptr_ = new ErrorHandler();
        }
        return shared_ptr_;
    }

    void ThrowError(int line, const std::string& info);
    void ThrowWarning(int line, const std::string& info);

    void Output();
};