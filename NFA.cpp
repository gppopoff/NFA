//
// Created by Jorko P on 09-May-19.
//

#include "NFA.h"
#include <string.h>
#include <iostream>
#include <algorithm>

const int SYMBOLS_IN_LANGUAGE = 36;

NFA::NFA() {
    strcpy(id,"ID***\0");
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

void NFA::addState(State newState) {
    states.addElement(newState);
    if(newState.isStateInitial()){
        initials.addElement(states.getNumberOfElements()-1);
    }
    if(newState.isStateFinal()){
        finals.addElement(states.getNumberOfElements()-1);
    }
}

bool NFA::recognize(const char * word,int length, int& symbolIndex) {
    return 0;
}

bool NFA::isAlreadyInStr(std::string str, int index) const {
    for(int i=0; i < str.length(); i++){
        if(str[i] - '0' == index){
            return true;
        }
    }
    return false;
}

bool NFA::isAlreadyInStrArr(Array<std::string> strArr, std::string str) const {
    for (int i = 0; i < strArr.getNumberOfElements(); i++) {
        if (strArr[i] == str) {
            return true;
        }
    }
    return false;
}
void NFA::epsilonclosure(std::string &new_state, int index) const {
    bool isThereE = false;
    if(!isAlreadyInStr(new_state,index))
    new_state += std::to_string(index);
    for (int i = 0; i < states[index].getTranss().getNumberOfElements() ; ++i) {
        if(states[index].getTranss()[i].getSymbol() == 'E'){
            isThereE = true;
            if(!isAlreadyInStr(new_state,states[index].getTranss()[i].getPath()))
            epsilonclosure(new_state,states[index].getTranss()[i].getPath());
        }
    }
    std::sort(new_state.begin(),new_state.end());
    if(!isThereE) {
        return;
    }


}

void NFA::epsilonclosureString(std::string &new_state, std::string old_state) const {
    for (int i = 0; i < old_state.length() ; i++) {
        epsilonclosure(new_state,old_state[i] - '0');
    }

}

std::string NFA::whereItGoesWith(std::string state, char symbol) const {
    std::string next_state;
    for(int i=0;i < state.length();i++){
        for (int j = 0; j < states[state[i] - '0'].getTranss().getNumberOfElements(); j++) {
            if(states[state[i] - '0'].getTranss()[j].getSymbol() == symbol){
                if(!isAlreadyInStr(next_state,states[state[i] - '0'].getTranss()[j].getPath())){
                    next_state += std::to_string(states[state[i] - '0'].getTranss()[j].getPath());
                    std::sort(next_state.begin(),next_state.end());
                }
            }
        }

    }
    if(next_state.empty()) next_state = std::to_string(-1);
    return next_state;
}

NFA NFA::detemine() const {
    NFA new_one;
    Array<std::string> new_states;
    std::string start;
    for (int i = 0; i < initials.getNumberOfElements(); i++) {
        epsilonclosure(start,initials[i]);
    }
    std::string stateWithSymbol;

    State newInitial;
    newInitial.makeInitial();
    new_states.addElement(start);
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
            if(!isAlreadyInStrArr(new_states,tempState)){
                new_states.addElement(tempState);
            }
            newInitial.addTrans(Trans(symbol,getIndexFromArr(new_states,tempState)));
        }else{
            newInitial.addTrans(Trans(symbol,-1));
        }
    }
    new_one.addState(newInitial);
    for (int i = 1; i < new_states.getNumberOfElements() ; i++) {
        State newState;
        for (int j = 0; j < SYMBOLS_IN_LANGUAGE ; j++) {
            std::string tempState;
            char symbol;
            if(j < 10){
                symbol = j + '0';
            }else{
                symbol = j + 'a' - 10;
            }
            stateWithSymbol = whereItGoesWith(new_states[i],symbol);
            if(stateWithSymbol != "-1"){
//                std::cout<<"\n--"<<stateWithSymbol<<"--\n";
                epsilonclosureString(tempState,stateWithSymbol);
//                std::cout<<tempState;
                if(!isAlreadyInStrArr(new_states,tempState)){
                    new_states.addElement(tempState);
                }
                newState.addTrans(Trans(symbol,getIndexFromArr(new_states,tempState)));
            }else{
                tempState = stateWithSymbol;
                newState.addTrans(Trans(symbol,-1));
            }
            for (int k = 0; k < finals.getNumberOfElements(); k++) {
                for (int l = 0; l < tempState.length(); l++) {
                    if(finals[k] == tempState[l] - '0'){
                        newState.makeFinal();
                    }
                }
            }
        }
        new_one.addState(newState);
    }
    new_one.print();
    return new_one;
}

void NFA::print() const {
    for(int i=0;i<states.getNumberOfElements();i++){
        if(states[i].isStateInitial()) std::cout<<"->";
        if(states[i].isStateFinal()) {
            std::cout << "(" << i << ")\n";
        }else std::cout<<i<<":\n";
        states[i].print();
        std::cout<<"------\n";
    }
}

int NFA::getIndexFromArr(Array<std::string> strArr, std::string str) const {
    for (int i = 0; i < strArr.getNumberOfElements(); i++) {
        if(strArr[i] == str){
            return i;
        }
    }
    return -1;
}




