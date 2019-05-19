//
// Created by Jorko P on 09-May-19.
//
#include "NFA.h"
#include <string.h>
#include <iostream>
#include <algorithm>

const int SYMBOLS_IN_LANGUAGE = 36;

bool NFA::isAlreadyInStr(const std::string& str, int index) const {
    for(char i : str){
        if(i - '0' == index){
            return true;
        }
    }
    return false;
}
bool NFA::isAlreadyInStrArr(Array<std::string> strArr,const std::string& str) const {
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
int NFA::getIndexFromArr(Array<std::string> strArr,const std::string& str) const {
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

NFA::NFA() {
    strcpy(id,"ID***\0");
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
const char* NFA::getId() const {
    return id;
}
void NFA::setId(char * newId) {
    strncpy(id,newId,5);
    id[5] = '\0';
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
    states[index].makeUnfinal();
    finals.removeElement(index);
}
void NFA::makeStateNotInitial(int index) {
    states[index].makeUninitial();
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

bool NFA::recognize(std::string word) {
    int stateIndex = 0;
    NFA temp = detemine();
    //temp.print();
    if(word.length() == 0){
        return temp.getStates()[0].isStateFinal();
    }
    for (int i = 0; i < word.length(); i++) {
        if(word[i] < 48 || word[i] > 122 || (word[i] > 57 && word[i] < 97)){
            std::cout<<"Unknown symbol";
            return false;
        }
        for (int j = 0; j < temp.getStates()[stateIndex].getTranss().getNumberOfElements(); j++) {

            if(word[i] == temp.getStates()[stateIndex].getTranss()[j].getSymbol()){
                //std::cout<< "We are in with:" << word[i]<< "\n";
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
    int stepsToState[numberOfStates];
    for (int l = 0; l < numberOfStates; l++) {
        stepsToState[l] = -1;
    }
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

    return true;
}
bool NFA::isDeterministic() const {
    if(initials.getNumberOfElements() > 1){
        return false;
    } else{
        for (int i = 0; i < states.getNumberOfElements(); i++) {
            bool pathExist[SYMBOLS_IN_LANGUAGE] = {false};
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
    NFA newOne;
    Array<std::string> newStates;
    std::string start;
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
//                std::cout<<"\n--"<<stateWithSymbol<<"--\n";
                epsilonclosureString(tempState,stateWithSymbol);
//                std::cout<<tempState;
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
    //newOne.print();
    return newOne;
}
NFA NFA::unite(const NFA& other) const {
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
