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
    char id[6]{};
    Array<int> finals;
    Array<int> initials;

    bool isAlreadyInStr(const std::string&, int) const;
    bool isAlreadyInStrArr(Array<std::string>,const std::string&) const;
    std::string whereItGoesWith(const std::string&, char) const;
    int getIndexFromArr(Array<std::string>,const std::string&) const;
    void epsilonclosure(std::string &new_state, int index) const;
    void epsilonclosureString(std::string&,const std::string&) const;
public:
    NFA();

    Array<State> getStates() const;
    Array<int> getInits() const ;
    Array<int> getFinals() const ;
    const char* getId() const;
    void setId(char[5]);
    void makeStateFinal(int);
    void makeStateInitial(int);
    void makeStateNotFinal(int);
    void makeStateNotInitial(int);
    void removeState(int);
    void addState(const State&);

    bool recognize(std::string);
    bool isLanguageEmpty() const;
    bool isDeterministic() const;
    NFA detemine() const;
    NFA unite(const NFA&) const ;
    NFA concat(const NFA&) const ;
    NFA un() const ;

    void print() const;

};


#endif //NFA_NFA_H
