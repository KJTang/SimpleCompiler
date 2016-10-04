#include "converter.h"

#include <algorithm>

Converter* Converter::shared_ptr_ = nullptr;

int Converter::ConvertStringToInt(const std::string& str) {
    int num = 0;
    if (str[0] == '-') {
        for (int i = 1; i < str.size(); ++i) {
            num *= 10;
            num += str[i] - '0';
        }
        num = -num;
    } else {
        for (int i = 0; i != str.size(); ++i) {
            num *= 10;
            num += str[i] - '0';
        }
    }
    return num;
}

double Converter::ConvertStringToDouble(const std::string& str) {
    double num = 0;
    double factor = 1;
    bool flag = false;
    for (int i = 0; i != str.size(); ++i) {
        if (str[i] == '.') {
            flag = true;
            continue;
        }
        num *= 10;
        num += str[i] - '0';
        if (flag) {
            factor *= 10;
        }
    }
    return num / factor;
}

std::string Converter::ConvertIntToString(int num) {
    if (num == 0) {
        return "0";
    }
    bool flag = false;
    if (num < 0) {
        flag = true;
        num = -num;
    }
    std::string str;
    while (num != 0) {
        str += (num % 10) + '0';
        num /= 10;
    }
    if (flag) {
        str += '-';
    }
    std::reverse(str.begin(), str.end());
    return str;
}

std::string Converter::ConvertDoubleToString(double num) {
    // TODO:
    return "0";
}