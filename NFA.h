//
// Created by Jorko P on 09-May-19.
//

#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "Array.h"
#include "State.h"
#include <string>

class NFA {
    Array<State> states;
    char id[6];
    Array<int> finals;
    Array<int> initials;

    bool isAlreadyInStr(std::string, int) const;
    bool isAlreadyInStrArr(Array<std::string>, std::string) const;
    std::string whereItGoesWith(std::string, char) const;
    int getIndexFromArr(Array<std::string>,std::string) const;
public:
    NFA();
    void epsilonclosure(std::string &new_state, int index) const;
    void epsilonclosureString(std::string&,std::string) const;
    NFA detemine() const;

    const char* getId() const;
    void setId(char[5]);

    void makeStateFinal(int);
    void makeStateInitial(int);
    void makeStateNotFinal(int);
    void makeStateNotInitial(int);
    void removeState(int);
    void addState(State);

    bool recognize(const char*,int,int&);

    void print() const;

};


#endif //NFA_NFA_H
