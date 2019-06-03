//
// Created by Jorko P on 09-May-19.
//

#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "Array.h"
#include "State.h"
#include <string>
#include <vector>

class NFA {
    Array<State> states;
    Array<int> finals;
    Array<int> initials;
    int id;
    static int nextId;

    bool isAlreadyInVec(const std::vector<int>&,int) const;
    bool isAlreadyInVecArr(const std::vector<std::vector<int>>& vecArr,const std::vector<int>& vec) const;

    std::vector<int> whereItGoesWith(const std::vector<int>&, char) const;
    int getIndexFromArr(const std::vector<std::vector<int>>& vecArr,const std::vector<int>& vec) const;
    void epsilonclosure(std::vector<int> &newState, int index) const;
    void epsilonclosureVector(std::vector<int> &newState,const std::vector<int> &oldState) const ;
    void addConcat(std::string&) const;
    bool isSymbolFromLanguage(char)const;
    bool lowerPriority(char,char) const;
    std::string toRPN(std::string&) const ;
    NFA performOperation(const NFA&,const NFA&,char) const;
    NFA performOperation(const NFA&,char) const;
    void getNextPermutation(std::string&) const;
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

    bool recognize(const std::string&) const ;
    bool isLanguageEmpty() const;
    bool isDeterministic() const;
    bool isLanguageInfinite() const;

    NFA detemine() const;
    NFA unite(const NFA&) const ;
    NFA concat(const NFA&) const ;
    NFA un() const ;

    void print() const;

};


#endif //NFA_NFA_H
