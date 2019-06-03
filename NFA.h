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
    Array<int> finals;
    Array<int> initials;
    int id;
    static int nextId;

    bool isAlreadyInStr(const std::string&, int) const;
    bool isAlreadyInStrArr(const Array<std::string>&,const std::string&) const;
    std::string whereItGoesWith(const std::string&, char) const;
    int getIndexFromArr(const Array<std::string>&,const std::string&) const;
    void epsilonclosure(std::string &new_state, int index) const;
    void epsilonclosureString(std::string&,const std::string&) const;
    void addConcat(std::string&) const;
    bool isSymbolFromLanguage(char)const;
    bool lowerPriority(char,char) const;
    std::string toRPN(std::string&) const ;
    NFA performOperation(const NFA&,const NFA&,char) const;
    NFA performOperation(const NFA&,char) const;
public:
    NFA();
    NFA(char);
    NFA(std::string&);

    static int getNextId();

    Array<State> getStates() const;
    Array<int> getInits() const ;
    Array<int> getFinals() const ;
    int getId() const;
    void makeStateFinal(int);
    void makeStateInitial(int);
    void makeStateNotFinal(int);
    void makeStateNotInitial(int);
    void removeState(int);
    void addState(const State&);

    bool recognize(const std::string&);
    bool isLanguageEmpty() const;
    bool isDeterministic() const;
    NFA detemine() const;
    NFA unite(const NFA&) const ;
    NFA concat(const NFA&) const ;
    NFA un() const ;

    void print() const;

};


#endif //NFA_NFA_H
