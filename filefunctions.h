//
// Created by Jorko P on 25-May-19.
//

#ifndef NFA_FILEFUNCTIONS_H
#define NFA_FILEFUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "NFA.h"

using namespace std;
int toInt(const string& str){
    int number = 0;
    for (int i = 0; i < str.length() ; i++) {
        number = number + str[str.length() - 1 - i] - '0' ;
    }
    cout<<number<<" ";
    return number;
}

vector<NFA>* convertToNFAs( ifstream& in){
    cout<<"We are in!\n";
    vector<NFA>* aumtomaton;
    while(in){
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
    }
    return 0;
}

void testReading(){
   /* bool runnig = true;
    string command;
    while(runnig){
        cin>>command;
        if(command == "exit"){ runnig = false; }
        else{
            if(command == "open"){
                ifstream in;
                string fileLocation, khh;
                //cin>>fileLocation;
                in.open("automaton.txt");
                convertToNFAs(in);

            }
        }

    }*/

}





#endif //NFA_FILEFUNCTIONS_H
