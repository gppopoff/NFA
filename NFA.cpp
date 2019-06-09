//
// Created by Jorko P on 09-May-19.
//
#include "NFA.h"
#include <string.h>
#include <iostream>
#include <algorithm>
#include <stack>
#include <cmath>
#include <vector>

const int SYMBOLS_IN_LANGUAGE = 36;
const char CONCAT_SYMBOL = '.';
const char UNION_SYMBOL = '+';
const char KLINIS_STAR = '*';


std::vector<int> NFA::whereItGoesWith(const std::vector<int>& state, char symbol) const {
    std::vector<int> nextState;
    for(int i : state){
        for (int j = 0; j < states[i].getTranss().getNumberOfElements(); j++) {
            if(states[i].getTranss()[j].getSymbol() == symbol){
                if(!isAlreadyInVec(nextState,states[i].getTranss()[j].getPath())){
                    nextState.push_back(states[i].getTranss()[j].getPath());
                    std::sort(nextState.begin(),nextState.end());
                }
            }
        }

    }
    if(nextState.empty()) nextState.push_back(-1);
    return nextState;
}
int NFA::getIndexFromArr(const std::vector<std::vector<int>>& vecArr,const std::vector<int>& vec) const {
    for (int i = 0; i < vecArr.size(); i++) {
        if(vecArr[i] == vec){
            return i;
        }
    }
    return -1;
}
void NFA::epsilonclosure(std::vector<int> &newState, int index) const {
    bool isThereE = false;
    if(!isAlreadyInVec(newState,index))
        newState.push_back(index);
    for (int i = 0; i < states[index].getTranss().getNumberOfElements() ; ++i) {
        if(states[index].getTranss()[i].getSymbol() == 'E'){
            isThereE = true;
            if(!isAlreadyInVec(newState,states[index].getTranss()[i].getPath()))
                epsilonclosure(newState,states[index].getTranss()[i].getPath());
        }
    }
    std::sort(newState.begin(),newState.end());
    if(!isThereE) {
        return;
    }
}
void NFA::epsilonclosureVector(std::vector<int> &newState,const std::vector<int> &oldState) const {
    for (int i : oldState) {
        epsilonclosure(newState,i);
    }

}
bool NFA::isSymbolFromLanguage(char element) const {
    return (element >= 'a' && element <= 'z') || (element >= '0' && element <= '9');
}
std::string NFA::addConcat(const std::string& RegExprOld) const {
    std::string RegExpr = RegExprOld;
    for (int i = 0; i < RegExpr.length() - 1 ; i++) {
        if((isSymbolFromLanguage(RegExpr[i]) && isSymbolFromLanguage(RegExpr[i+1]))
        || (isSymbolFromLanguage(RegExpr[i]) && RegExpr[i+1] == '(')
        || (isSymbolFromLanguage(RegExpr[i+1]) && RegExpr[i] == ')')
        || ((RegExpr[i] == '*') && (RegExpr[i+1] == '(' || isSymbolFromLanguage(RegExpr[i+1])))){
            RegExpr.insert(i+1,".");
        }
    }
    return RegExpr;
}
bool NFA::lowerPriority(char first, char second) const {
    return (first == CONCAT_SYMBOL && second == KLINIS_STAR)
           || (first == UNION_SYMBOL && second == CONCAT_SYMBOL)
           || (first == UNION_SYMBOL && second == KLINIS_STAR);
}
std::string NFA::toRPN(std::string& RegExpr) const {
    std::string regExprInRPN;
    std::stack<char> operations;
    for (char i : RegExpr) {
        if(isSymbolFromLanguage(i)){
            regExprInRPN+=i;
        }
        else{
            if(operations.empty()){
                operations.push(i);
            }
            else if(i== '('){
                operations.push('(');
            }
            else if(i== ')'){
                while(operations.top()!= '('){
                    regExprInRPN += operations.top();
                    operations.pop();
                }
                operations.pop();
            }
            else if(!lowerPriority(i,operations.top())){
                operations.push(i);
            }else if(lowerPriority(i,operations.top())){
                while(!operations.empty() && operations.top() != i ){
                    regExprInRPN += operations.top();
                    operations.pop();
                }
                operations.push(i);
            }

        }
    }
    while(!operations.empty()){
        regExprInRPN += operations.top();
        operations.pop();
    }
    return  regExprInRPN;
}
NFA NFA::performOperation(const NFA & first, const NFA & second, char operation) const {
    if(operation == UNION_SYMBOL){
//        first.unite(second).print();
//        std::cout<<"==========================================\n";
        return first.unite(second);
    }else{
//        first.concat(second).print();
//        std::cout<<"==========================================\n";
        return first.concat(second);
    }
}
NFA NFA::performOperation(const NFA & first, char operation) const {
//    first.un().print();
//    std::cout<<"==========================================\n";
    if(operation == KLINIS_STAR)
    return first.un();
    else return 0;
}
void NFA::getNextPermutation(std::string& word) const {

    for (int i = word.length() - 1; i >= 0 ; i--) {
        if(word[i] + 1 > 'z'){
            word[i] = '0';
        }else{
            if(word[i] < '9' || word[i]>= 'a'){
                word[i] += 1;
            }else{
                word[i] = 'a';
            }
            break;
        }
    }
}


int NFA::nextId = 1;
NFA::NFA() {
    id = getNextId();
}
NFA::NFA(char symbol) {
    id = getNextId();
    State initial,final;
    initial.addTrans(Trans(symbol,1));
//    initial.makeInitial();
//    final.makeFinal();
    states.addElement(initial);
    states.addElement(final);
    makeStateInitial(0);
    makeStateFinal(1);
}

NFA::NFA(const std::string& RegExpr) {
    // here we will make a automaton from regular expression
    id = getNextId();
    std::string coolOne = addConcat(RegExpr);
    std::string regExprInRPN = toRPN(coolOne);
    std::stack<NFA> operands;
    for(char i: regExprInRPN){
        if(isSymbolFromLanguage(i)){
            operands.push(NFA(i));
        }else if(i == KLINIS_STAR){
            NFA first = operands.top();
            operands.pop();
            operands.push(performOperation(first,KLINIS_STAR));
        }else{
            NFA second = operands.top();
            operands.pop();
            NFA first = operands.top();
            operands.pop();
            operands.push(performOperation(first,second,i));
        }
    }
    *this = operands.top();
}
int NFA::getNextId() {
    return nextId++;
}
Array<State> NFA::getStates() const {
    return states;
}
Array<int> NFA::getInits() const {
    return initials;
}
Array<int> NFA::getFinals() const {
    return finals;
}

int NFA::getId() const {
    return id;
}
void NFA::makeStateFinal(int index) {
    states[index].makeFinal();
    finals.addElement(index);
}
void NFA::makeStateInitial(int index) {
    states[index].makeInitial();
    initials.addElement(index);
}
void NFA::makeStateNotFinal(int index) {
    states[index].makeNotFinal();
    finals.removeElement(index);
}
void NFA::makeStateNotInitial(int index) {
    states[index].makeNotInitial();
    initials.removeElement(index);
}
void NFA::removeState(int index) {
    states.removeElement(states[index]);
}

void NFA::addState(const State& newState) {
    states.addElement(newState);
    if(newState.isStateInitial()){
        initials.addElement(states.getNumberOfElements()-1);
    }
    if(newState.isStateFinal()){
        finals.addElement(states.getNumberOfElements()-1);
    }
}
bool NFA::recognize(const std::string& word) const {
    int stateIndex = 0;
    NFA temp;
    if(!isDeterministic()){
        temp = detemine();
    }
    else{
        temp = *this;
    }
    if(word.length() == 0){
        return temp.getStates()[0].isStateFinal();
    }
    if(temp.isLanguageEmpty()){
        return false;
    }
    for (int i = 0; i < word.length(); i++) {
        if(word[i] < 48 || word[i] > 122 || (word[i] > 57 && word[i] < 97)){
            std::cout<<"Unknown symbol";
            return false;
        }
        //For every character of the word we check if in the current state there is
        //transition with that character
        for (int j = 0; j < temp.getStates()[stateIndex].getTranss().getNumberOfElements(); j++) {
            if(word[i] == temp.getStates()[stateIndex].getTranss()[j].getSymbol()){
                if(temp.getStates()[stateIndex].getTranss()[j].getPath() == -1){
                    return false;
                } else {
                    stateIndex = temp.getStates()[stateIndex].getTranss()[j].getPath();
                    if(temp.getStates()[stateIndex].isStateFinal() && i == word.length()-1){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
bool NFA::isLanguageEmpty() const{
    NFA temp = detemine();
    const int numberOfStates = temp.getStates().getNumberOfElements();
    int* stepsToState = new int[numberOfStates];
    for (int l = 0; l < numberOfStates; l++) {
        stepsToState[l] = -1;
    }
    // Kind of BFS ??? if we have reached the final state its OK
    stepsToState[0] = 0;
    bool madeChange = true;
    for (int i = 0; madeChange ; i++) {
        madeChange = false;
        for (int k = 0; k < numberOfStates; k++) {
            if(stepsToState[k] == i){
                for (int j = 0; j < temp.getStates()[k].getTranss().getNumberOfElements(); j++) {
                    if(temp.getStates()[k].getTranss()[j].getPath() != -1 &&
                       stepsToState[temp.getStates()[k].getTranss()[j].getPath()] == -1 ){
                        if(temp.getStates()[k].isStateFinal()){
                            return false;
                        }
                        stepsToState[temp.getStates()[k].getTranss()[j].getPath()] = i;
                        madeChange = true;
                    }
                }
            }
        }

    }
    delete[] stepsToState;
    return true;
}
bool NFA::isDeterministic() const {
    if(initials.getNumberOfElements() > 1){
        return false;
    } else{
        for (int i = 0; i < states.getNumberOfElements(); i++) {
            bool pathExist[SYMBOLS_IN_LANGUAGE] = {false};
            // for each state we check if there is Epsilon-transition or
            // if we already have transition with that symbol
            for (int j = 0; j < states[i].getTranss().getNumberOfElements(); j++) {
                if(states[i].getTranss()[j].getSymbol() == 'E' ||
                        pathExist[states[i].getTranss()[j].getPath()] != 0){
                    return false;
                } else{
                    if(states[i].getTranss()[j].getPath()!= -1)
                    pathExist[states[i].getTranss()[j].getPath()] = true;
                }
            }
        }
    }
    return true;
}

bool NFA::isLanguageInfinite() const {
    NFA Copy;
    if(!isDeterministic()){
        Copy = this->detemine();
    }else{
        Copy = *this;
    }
    for (int i = Copy.getStates().getNumberOfElements();
    i < 2*Copy.getStates().getNumberOfElements();
    i++) {
        std::string word;
        for (int k = 0; k < i; k++) {
            word += "0";
        }
        for (int j = 0; j < pow(i,i) ; j++) {
            if(Copy.recognize(word)){
                return true;
            } else{

                getNextPermutation(word);
            }
        }
    }
    return false;
}
NFA NFA::detemine() const {
    if(this->isDeterministic()){
        return *this;
    }
    NFA newOne;
    std::vector<std::vector<int>> newStates;
    std::vector<int> start;

    // Standard way to determine an automaton with epsilon transitions
    for (int i = 0; i < initials.getNumberOfElements(); i++) {
        epsilonclosure(start,initials[i]);
    }
    std::vector<int> stateWithSymbol;
    State newInitial;
    newInitial.makeInitial();
    newStates.push_back(start);
    for (int i = 0; i < SYMBOLS_IN_LANGUAGE ; i++) {
        std::vector<int> tempState;
        char symbol;
        if(i < 10){
            symbol = i + '0';
        }else{
            symbol = i + 'a' - 10;
        }
        stateWithSymbol = whereItGoesWith(start,symbol);

        if(stateWithSymbol[0] != -1){
            epsilonclosureVector(tempState,stateWithSymbol);
            if(!isAlreadyInVecArr(newStates,tempState)){
                newStates.push_back(tempState);
            }
            newInitial.addTrans(Trans(symbol,getIndexFromArr(newStates,tempState)));
        }else{
            newInitial.addTrans(Trans(symbol,-1));
        }
        for (int k = 0; k < finals.getNumberOfElements(); k++) {
            for (int l : tempState) {
                if(finals[k] == l ){
                    newInitial.makeFinal();
                }
            }
        }
    }
    newOne.addState(newInitial);
    for (int i = 1; i < newStates.size() ; i++) {
        State newState;
        for (int j = 0; j < SYMBOLS_IN_LANGUAGE ; j++) {
            std::vector<int> tempState;
            char symbol;
            if(j < 10){
                symbol = j + '0';
            }else{
                symbol = j + 'a' - 10;
            }
            stateWithSymbol = whereItGoesWith(newStates[i],symbol);
            if(stateWithSymbol[0] != -1){
                epsilonclosureVector(tempState,stateWithSymbol);
                if(!isAlreadyInVecArr(newStates,tempState)){
                    newStates.push_back(tempState);
                }
                newState.addTrans(Trans(symbol,getIndexFromArr(newStates,tempState)));
            }else{
                tempState = stateWithSymbol;
                newState.addTrans(Trans(symbol,-1));
            }

            for (int k = 0; k < finals.getNumberOfElements(); k++) {
                for (int l : tempState) {
                    if(finals[k] == l){
                        newState.makeFinal();
                    }
                }
            }
        }
        if (i == 0) newState.makeInitial();
        newOne.addState(newState);
    }
    return newOne;
}
NFA NFA::unite(const NFA& other) const {
    // Adding the states from the other automaton and making them finals and initials
    const int numberOfStates = this->getStates().getNumberOfElements();
    NFA newOne = *this;
    for (int i = 0; i < other.getStates().getNumberOfElements() ; i++) {
        State temp;
        for (int j = 0;j < other.getStates()[i].getTranss().getNumberOfElements() ; j++) {
            temp.addTrans(Trans(other.getStates()[i].getTranss()[j].getSymbol(),
                    other.getStates()[i].getTranss()[j].getPath()+ numberOfStates));
        }
        newOne.addState(temp);
    }
    for (int j = 0; j < other.getInits().getNumberOfElements(); j++) {
        newOne.makeStateInitial(other.getInits()[j] + numberOfStates);
    }
    for (int j = 0; j < other.getFinals().getNumberOfElements(); j++) {
        newOne.makeStateFinal(other.getFinals()[j] + numberOfStates);
    }
    return newOne;
}

NFA NFA::concat(const NFA& other) const {
    //Adding the states from the other automaton, saving others finals but removing initials,
    // adding initials from the first one
    // and adding Epsilon-transition from first finals to others initials
    const int numberOfStates = other.getStates().getNumberOfElements();
    NFA newOne = other;
    for (int i = 0; i < this->getStates().getNumberOfElements() ; i++) {
        State temp;
        for (int j = 0;j < this->getStates()[i].getTranss().getNumberOfElements() ; j++) {
            temp.addTrans(Trans(this->getStates()[i].getTranss()[j].getSymbol(),
                                this->getStates()[i].getTranss()[j].getPath()+ numberOfStates));
        }
        if(this->getStates()[i].isStateInitial()){
            temp.makeInitial();
        }
        if(this->getStates()[i].isStateFinal()){
            for (int j = 0; j < other.getInits().getNumberOfElements() ; j++) {
                temp.addTrans(Trans('E',other.getInits()[j]));
            }
        }
        newOne.addState(temp);
    }
    for (int k = 0; k < other.getInits().getNumberOfElements(); k++) {
        newOne.makeStateNotInitial(other.getInits()[k]);
    }
    return newOne;
}

NFA NFA::un() const {
    // Adding Epsion-transitions from finals states to initials
    NFA newOne;
    for (int i = 0; i < this->getStates().getNumberOfElements() ; i++) {
        State temp;
        for (int j = 0;j < this->getStates()[i].getTranss().getNumberOfElements() ; j++) {
            temp.addTrans(Trans(this->getStates()[i].getTranss()[j].getSymbol(),
                                this->getStates()[i].getTranss()[j].getPath()));
        }
        if(this->getStates()[i].isStateInitial()){
            temp.makeInitial();
        }
        if(this->getStates()[i].isStateFinal()){
            temp.makeFinal();
            for (int j = 0; j < this->getInits().getNumberOfElements() ; j++) {
                temp.addTrans(Trans('E',this->getInits()[j]));
            }
        }
        newOne.addState(temp);
    }
    State epsilon;
    epsilon.makeFinal();
    epsilon.makeInitial();
    newOne.addState(epsilon);
    return newOne;
}

void NFA::print() const {
    std::cout<<"Automata Id:"<<id<<"\n";
    for(int i=0;i<states.getNumberOfElements();i++){
        if(states[i].isStateInitial()) std::cout<<"->";
        if(states[i].isStateFinal()) {
            std::cout << "(" << i << ")\n";
        }else std::cout<<i<<":\n";
        states[i].print();
        std::cout<<"\n------\n";
    }
}

bool NFA::isAlreadyInVec(const std::vector<int> & vec, int state) const {
    for (int i : vec) {
        if(i == state){
            return true;
        }
    }
    return false;
}
bool NFA::isAlreadyInVecArr(const std::vector<std::vector<int>>& vecArr,const std::vector<int>& vec) const {
    for (const auto & i : vecArr) {
        if (i == vec) {
            return true;
        }
    }
    return false;
}

NFA &NFA::operator=(const NFA& other) {
    if(this != &other){
        states = other.states;
        finals = other.finals;
        initials = other.initials;
    }
    return *this;
}






