#include "converter.h"

#include <algorithm>

Converter* Converter::shared_ptr_ = nullptr;

int Converter::ConvertStringToInt(const std::string& str) {
    // int num = 0;
    // if (str[0] == '-') {
    //     for (int i = 1; i < str.size(); ++i) {
    //         num *= 10;
    //         num += str[i] - '0';
    //     }
    //     num = -num;
    // } else {
    //     for (int i = 0; i != str.size(); ++i) {
    //         num *= 10;
    //         num += str[i] - '0';
    //     }
    // }
    // return num;
    return std::stoi(str);
}

double Converter::ConvertStringToDouble(const std::string& str) {
    // double num = 0;
    // double factor = 1;
    // bool dot_flag = false;
    // if (str[0] == '-') {
    //     for (int i = 1; i < str.size(); ++i) {
    //         if (str[i] == '.') {
    //             dot_flag = true;
    //             continue;
    //         }
    //         num *= 10;
    //         num += str[i] - '0';
    //         if (dot_flag) {
    //             factor *= 10;
    //         }
    //     }
    //     num = -num;
    // } else {
    //     for (int i = 0; i != str.size(); ++i) {
    //         if (str[i] == '.') {
    //             dot_flag = true;
    //             continue;
    //         }
    //         num *= 10;
    //         num += str[i] - '0';
    //         if (dot_flag) {
    //             factor *= 10;
    //         }
    //     }
    // }
    // return num / factor;
    return std::stod(str);
}

std::string Converter::ConvertIntToString(int num) {
    // if (num == 0) {
    //     return "0";
    // }
    // bool neg_flag = false;
    // if (num < 0) {
    //     neg_flag = true;
    //     num = -num;
    // }
    // std::string str;
    // while (num != 0) {
    //     str += (num % 10) + '0';
    //     num /= 10;
    // }
    // if (neg_flag) {
    //     str += '-';
    // }
    // std::reverse(str.begin(), str.end());
    // return str;
    return std::to_string(num);
}

std::string Converter::ConvertDoubleToString(double num) {
    return std::to_string(num);
}