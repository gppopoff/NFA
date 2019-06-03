//
// Created by Jorko P on 09-May-19.
//
#include "NFA.h"
#include <string.h>
#include <iostream>
#include <algorithm>
#include <stack>

const int SYMBOLS_IN_LANGUAGE = 36;
const char CONCAT_SYMBOL = '.';
const char UNION_SYMBOL = '+';
const char KLINIS_STAR = '*';

bool NFA::isAlreadyInStr(const std::string& str, int index) const {
    for(char i : str){
        if(i - '0' == index){
            return true;
        }
    }
    return false;
}
bool NFA::isAlreadyInStrArr(const Array<std::string>& strArr,const std::string& str) const {
    for (int i = 0; i < strArr.getNumberOfElements(); i++) {
        if (strArr[i] == str) {
            return true;
        }
    }
    return false;
}
std::string NFA::whereItGoesWith(const std::string& state, char symbol) const {
    std::string nextState;
    for(char i : state){
        for (int j = 0; j < states[i - '0'].getTranss().getNumberOfElements(); j++) {
            if(states[i - '0'].getTranss()[j].getSymbol() == symbol){
                if(!isAlreadyInStr(nextState,states[i - '0'].getTranss()[j].getPath())){
                    nextState += std::to_string(states[i - '0'].getTranss()[j].getPath());
                    std::sort(nextState.begin(),nextState.end());
                }
            }
        }

    }
    if(nextState.empty()) nextState = std::to_string(-1);
    return nextState;
}
int NFA::getIndexFromArr(const Array<std::string>& strArr,const std::string& str) const {
    for (int i = 0; i < strArr.getNumberOfElements(); i++) {
        if(strArr[i] == str){
            return i;
        }
    }
    return -1;
}
void NFA::epsilonclosure(std::string &newState, int index) const {
    bool isThereE = false;
    if(!isAlreadyInStr(newState,index))
        newState += std::to_string(index);
    for (int i = 0; i < states[index].getTranss().getNumberOfElements() ; ++i) {
        if(states[index].getTranss()[i].getSymbol() == 'E'){
            isThereE = true;
            if(!isAlreadyInStr(newState,states[index].getTranss()[i].getPath()))
                epsilonclosure(newState,states[index].getTranss()[i].getPath());
        }
    }
    std::sort(newState.begin(),newState.end());
    if(!isThereE) {
        return;
    }
}
void NFA::epsilonclosureString(std::string &newState,const std::string& oldState) const {
    for (char i : oldState) {
        epsilonclosure(newState,i - '0');
    }

}
bool NFA::isSymbolFromLanguage(char element) const {
    return (element >= 'a' && element <= 'z') || (element >= '0' && element <= '9');
}
void NFA::addConcat(std::string& RegExpr) const {
    for (int i = 0; i < RegExpr.length() - 1 ; i++) {
        if((isSymbolFromLanguage(RegExpr[i]) && isSymbolFromLanguage(RegExpr[i+1]))
        || (isSymbolFromLanguage(RegExpr[i]) && RegExpr[i+1] == '(')
        || (isSymbolFromLanguage(RegExpr[i+1]) && RegExpr[i] == ')')
        || ((RegExpr[i] == '*') && (RegExpr[i+1] == '(' || isSymbolFromLanguage(RegExpr[i+1])))){
            RegExpr.insert(i+1,".");
        }
    }
    std::cout<<RegExpr<<"\n";
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
    std::cout<<regExprInRPN<<"\n";
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
    return first.un();
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

NFA::NFA(std::string& RegExpr) {
    // here we will make a automaton from regular expression
    id = getNextId();
    addConcat(RegExpr);
    std::string regExprInRPN = toRPN(RegExpr);
    std::stack<NFA> operands;
    for(char i: regExprInRPN){
        if(isSymbolFromLanguage(i)){
            operands.push(NFA(i));
        }else if(i == KLINIS_STAR){
            NFA first = operands.top();
            operands.pop();
            operands.push(performOperation(first,KLINIS_STAR));
        }else{
            NFA first = operands.top();
            operands.pop();
            NFA second = operands.top();
            operands.pop();
            operands.push(performOperation(second,first,i));
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
bool NFA::recognize(const std::string& word) {
    int stateIndex = 0;
    NFA temp = detemine();
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

NFA NFA::detemine() const {
    if(this->isDeterministic()){
        return *this;
    }
    NFA newOne;
    Array<std::string> newStates;
    std::string start;

    // Standard way to determine an automaton with epsilon transitions
    for (int i = 0; i < initials.getNumberOfElements(); i++) {
        epsilonclosure(start,initials[i]);
    }
    std::string stateWithSymbol;
    State newInitial;
    newInitial.makeInitial();
    newStates.addElement(start);
    for (int i = 0; i < SYMBOLS_IN_LANGUAGE ; i++) {
        std::string tempState;
        char symbol;
        if(i < 10){
            symbol = i + '0';
        }else{
            symbol = i + 'a' - 10;
        }
        stateWithSymbol = whereItGoesWith(start,symbol);
        if(stateWithSymbol != "-1"){
            epsilonclosureString(tempState,stateWithSymbol);
            if(!isAlreadyInStrArr(newStates,tempState)){
                newStates.addElement(tempState);
            }
            newInitial.addTrans(Trans(symbol,getIndexFromArr(newStates,tempState)));
        }else{
            newInitial.addTrans(Trans(symbol,-1));
        }
        for (int k = 0; k < finals.getNumberOfElements(); k++) {
            for (char l : tempState) {
                if(finals[k] == l - '0'){
                    newInitial.makeFinal();
                }
            }
        }
    }
    newOne.addState(newInitial);
    for (int i = 1; i < newStates.getNumberOfElements() ; i++) {
        State newState;
        for (int j = 0; j < SYMBOLS_IN_LANGUAGE ; j++) {
            std::string tempState;
            char symbol;
            if(j < 10){
                symbol = j + '0';
            }else{
                symbol = j + 'a' - 10;
            }
            stateWithSymbol = whereItGoesWith(newStates[i],symbol);
            if(stateWithSymbol != "-1"){
                epsilonclosureString(tempState,stateWithSymbol);
                if(!isAlreadyInStrArr(newStates,tempState)){
                    newStates.addElement(tempState);
                }
                newState.addTrans(Trans(symbol,getIndexFromArr(newStates,tempState)));
            }else{
                tempState = stateWithSymbol;
                newState.addTrans(Trans(symbol,-1));
            }

            for (int k = 0; k < finals.getNumberOfElements(); k++) {
                for (char l : tempState) {
                    if(finals[k] == l - '0'){
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
    for(int i=0;i<states.getNumberOfElements();i++){
        if(states[i].isStateInitial()) std::cout<<"->";
        if(states[i].isStateFinal()) {
            std::cout << "(" << i << ")\n";
        }else std::cout<<i<<":\n";
        states[i].print();
        std::cout<<"\n------\n";
    }
}







