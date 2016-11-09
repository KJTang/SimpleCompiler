#pragma once

#include <string>

class Converter {
private:
    static Converter* shared_ptr_;
public:
    Converter() {}
    ~Converter() {}
    
    static Converter* GetInstance() {
        if (!shared_ptr_) {
            shared_ptr_ = new Converter();
        }
        return shared_ptr_;
    }

    int ConvertStringToInt(const std::string& str);
    double ConvertStringToDouble(const std::string& str);
    std::string ConvertIntToString(int num);
    std::string ConvertDoubleToString(double num);
};
