#ifndef COMPLEXTYPE_H
#define COMPLEXTYPE_H

#include <string>

struct ComplexType {
    int a;
    double b;
    std::string c;

    ComplexType(int a, double b, const std::string& c);
};

ComplexType::ComplexType(int a, double b, const std::string& c) : a(a), b(b), c(c) {}

#endif // COMPLEXTYPE_H