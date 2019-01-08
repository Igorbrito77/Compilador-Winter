#ifndef CONVERSAO_PRECISA_H 
#define CONVERSAO_PRECISA_H 
#include <iostream>
#include <iomanip>

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}


#endif