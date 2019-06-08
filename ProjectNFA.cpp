//
// Created by Jorko P on 08-Jun-19.
//

#include "ProjectNFA.h"
#include <iostream>
#include <fstream>

void ProjectNFA::open(const std::string& fileName) {
    std::ifstream in(fileName);
    if(in.good()){
        NFA automata;
        int numberOfStates;
        in>>numberOfStates;
        State* states = new State[numberOfStates];
        for (int i = 0; i < numberOfStates ; i++) {
            int state;
            int numberOfTrans;
            in>>state;
            in>>numberOfTrans;
            for (int j = 0; j < numberOfTrans; j++) {
                int transToState;
                char symbol;
                in>>symbol;
                in>>transToState;
                states[state].addTrans(Trans(symbol,transToState));
//                states[state].print();
            }
        }
        int numOfFinals;
        in>>numOfFinals;
        for (int k = 0; k < numOfFinals; k++) {
            int state;
            in>>state;
            states[state].makeFinal();
        }
        int numOfInits;
        in>>numOfInits;
        for (int k = 0; k < numOfInits; k++) {
            int state;
            in>>state;
            states[state].makeInitial();
        }
        for (int l = 0; l < numberOfStates; l++) {
            automata.addState(states[l]);
        }

        delete[] states;
        std::cout<<"File \""<<fileName<<"\" successfully opened.\n";
        list.push_back(automata);
    } else{
        std::cout<<"Error! Invalid file name\n";
    }
}

void ProjectNFA::listNFAs() const {
    std::cout<<"Current IDs:\n";
    for (const auto & i : list) {
        std::cout<<i.getId()<<". ";
    }
    std::cout<<"\n";
}

void ProjectNFA::printNFA(int id) const {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id){
            i.print();
            printed = true;
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::saveNFA(int id, const std::string &fileName) const {

    bool opened = false;
    for(const auto& i: list){
        if(i.getId() == id){
            std::ofstream out(fileName);
            out<< i.getStates().getNumberOfElements() <<"\n";
            for (int j = 0; j < i.getStates().getNumberOfElements(); j++) {
                out<<j<<" "<<i.getStates()[j].getTranss().getNumberOfElements()<<" ";
                for (int k = 0; k < i.getStates()[j].getTranss().getNumberOfElements(); k++) {
                    out<<i.getStates()[j].getTranss()[k].getSymbol()<<" ";
                    out<<i.getStates()[j].getTranss()[k].getPath()<<" ";
                }
                out<<"\n";
            }
            out<<i.getFinals().getNumberOfElements()<<" ";
            for (int l = 0; l <i.getFinals().getNumberOfElements() ; l++) {
                out<<i.getFinals()[l]<<" ";
            }
            out<<"\n";

            out<<i.getFinals().getNumberOfElements()<<" ";
            for (int l = 0; l <i.getInits().getNumberOfElements() ; l++) {
                out<<i.getInits()[l]<<" ";
            }
            opened = true;
            break;
        }
    }
    if(!opened){
        std::cout<<"Invalid ID!\n";
    }

}

void ProjectNFA::emptyNFA(int id) const {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id){
            if(i.isLanguageEmpty()){
                std::cout<<"Language of automata with ID:"<<id<<" is empty.\n";
            }else{
                std::cout<<"Language of automata with ID:"<<id<<" is not empty.\n";
            }
            printed = true;
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::deterministicNFA(int id) const {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id){
            if(i.isDeterministic()){
                std::cout<<"Automata with ID:"<<id<<" is deterministic.\n";
            }else{
                std::cout<<"Automata with ID:"<<id<<" is not deterministic.\n";
            }
            printed = true;
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::recognizeWord(int id, const std::string & word) const {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id){
            if(i.recognize(word)){
                std::cout<<"Automata with ID:"<<id<<" recognizes the word:"<<word<<".\n";
            }else{
                std::cout<<"Automata with ID:"<<id<<"do not recognizes the word:"<<word<<".\n";
            }
            printed = true;
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::unionNFAs(int id1, int id2) {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id1){
            for(const auto& j: list){
                if(j.getId() == id2){
                    NFA newOne;
                    newOne = i.unite(j);
                    list.push_back(newOne);
                    std::cout<<"New automata added with ID:"<<newOne.getId()<<".\n";
                    printed = true;
                    break;
                    }
                }
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::concatNFAs(int id1, int id2) {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id1){
            for(const auto& j: list){
                if(j.getId() == id2){
                    NFA newOne;
                    newOne = i.concat(j);
                    list.push_back(newOne);
                    std::cout<<"New automata added with ID:"<<newOne.getId()<<".\n";
                    printed = true;
                    break;
                }
            }
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::unNFA(int id) {
    bool printed = false;
    for(const auto& j: list){
        if(j.getId() == id){
            NFA newOne;
            newOne = j.un();
            list.push_back(newOne);
            std::cout<<"New automata added with ID:"<<newOne.getId()<<".\n";
            printed = true;
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::reg(const std::string& word) {
    NFA newOne(word);
    list.push_back(newOne);
    std::cout<<"New automata added with ID:"<<newOne.getId()<<".\n";

}

void ProjectNFA::InfiniteNFA(int id) const {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id){
            if(i.isLanguageInfinite()){
                std::cout<<"Language of automata with ID:"<<id<<" is infinite.\n";
            }else{
                std::cout<<"Language of automata with ID:"<<id<<" is finite.\n";
            }
            printed = true;
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}
