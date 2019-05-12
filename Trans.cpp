//
// Created by Jorko P on 09-May-19.
//

#include "Trans.h"
#include <iostream>

Trans::Trans(char s, int p) : symbol(s), path(p) {}

void Trans::setSymbol(char s) {
    symbol = s;
}
void Trans::setPath(int p) {
    path = p;
}
void Trans::print() const {
    std::cout<<symbol<< "->" << path << "  ";
}
bool Trans::operator==(const Trans & other) const {
    if(symbol == other.getSymbol() && path == other.getPath()){
        return 1;
    }
    return 0;
}