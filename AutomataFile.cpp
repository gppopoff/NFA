//
// Created by Jorko P on 31-May-19.
//

#include "AutomataFile.h"
#include "NFA.h"
#include <fstream>
#include <iostream>

void AutomataFile::open(const std::string& directoryPath) {
    fileOpened = true;
    fileName = directoryPath;
    std::ifstream in(fileName);
    if(in.good()){
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
                states[i].addTrans(Trans(symbol,transToState));
                states[i].print();
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
        std::cout<<"Successfully opened a file";
    }else{
        std::cout<<"Error! Incorrect file path.";
    }


}
