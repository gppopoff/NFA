//
// Created by Jorko P on 09-May-19.
//

#include "State.h"
#include <iostream>

State::State() {
    makeUnfinal();
    makeUninitial();
}
void State::setTranss(const Array<Trans> &other) {
    transitions = other;
}
void State::makeFinal() {
    isFinal = true;
}
void State::makeUnfinal() {
    isFinal = false;
}
void State::makeInitial() {
    isInitial = true;
}
void State::makeUninitial() {
    isInitial = false;
}

void State::addTrans(Trans newTrans) {
    transitions.addElement(newTrans);
}
void State::removeTrans(Trans oldTrans) {
   /* for(int i = 0 ; i < transitions.getNumberOfElements() ; i++){
        if(transitions[i] == oldTrans){
            for(int j = i; j < transitions.getNumberOfElements() - 1; j++){
                transitions[j] = transitions[j+1];
            }
            transitions.reduceNumberOfElements();
            break;
        }
    }*/
   transitions.removeElement(oldTrans);
}

void State::print() const {
    for(int i = 0; i < transitions.getNumberOfElements(); i++){
        transitions[i].print();
    }
    if(isFinal)std::cout<<"Final\n";
    if(isInitial)std::cout<<"Initial\n";
}

bool State::operator==(const State & other) const {
    if(transitions == other.getTranss() && isFinal == other.isFinal && isInitial == other.isInitial){
        return true;
    }
    return false;
}
