//
// Created by Jorko P on 08-Jun-19.
//

#include "ProjectNFA.h"
#include <iostream>
#include <fstream>
#include <regex>

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
        std::cout<<"New automata added with ID:"<<automata.getId()<<".\n";
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

            out<<i.getInits().getNumberOfElements()<<" ";
            for (int l = 0; l <i.getInits().getNumberOfElements() ; l++) {
                out<<i.getInits()[l]<<" ";
            }
            opened = true;
            std::cout<<"Successfully saved automata with ID:"<<id<<" in \""<<fileName<<"\".\n";
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

void ProjectNFA::determineNFA(int id) {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id){
            NFA newOne;
            newOne = i.detemine();
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

void ProjectNFA::recognizeWord(int id, const std::string & word) const {
    bool printed = false;
    for(const auto& i: list){
        if(i.getId() == id){
            if(i.recognize(word)){
                std::cout<<"Automata with ID:"<<id<<" recognizes the word:\""<<word<<"\".\n";
            }else{
                std::cout<<"Automata with ID:"<<id<<" do not recognizes the word:\""<<word<<"\".\n";
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
                    /*NFA newOne;
                    newOne = i.unite(j);
                    i.unite(j).print();*/
                    list.push_back(i.unite(j));
                    std::cout<<"New automata added with ID:"<<list.back().getId()<<".\n";
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
/*                    NFA newOne;
                    newOne = i.concat(j);*/
                    list.push_back(i.concat(j));
                    std::cout<<"New automata added with ID:"<<list.back().getId()<<".\n";
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
/*            NFA newOne;
            newOne = j.un();*/
            list.push_back(j.un());
            std::cout<<"New automata added with ID:"<<list.back().getId()<<".\n";
            printed = true;
            break;
        }
    }
    if(!printed){
        std::cout<<"Invalid ID!\n";
    }
}

void ProjectNFA::reg(const std::string& word) {
    for (int i = 0; i < word.length(); i++) {

    }
    NFA newOne(word);
    list.push_back(newOne);
    std::cout<<"New automata added with ID:"<<newOne.getId()<<".\n";

}

void ProjectNFA::infiniteNFA(int id) const {
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

void ProjectNFA::Activate() {

    const std::string openRegExpr = R"(^open (([^\"\s]+)|(\"([^\"]+)\"))$)";
    const std::string listRegExpr = R"(^list$)";
    const std::string printRegExpr = R"(^print ([0-9]+)$)";
    const std::string saveRegExpr = R"(^save ([0-9]+) (([^\"\s]+)|(\"([^\"]+)\"))$)";
    const std::string exitRegExpr = R"(^exit$)";
    const std::string emptyRegExpr = R"(^empty ([0-9]+)$)";
    const std::string infiniteRegExpr = R"(^infinite ([0-9]+)$)";
    const std::string deterministicRegExpr = R"(^deterministic ([0-9]+)$)";
    const std::string determineRegExpr = R"(^determine ([0-9]+)$)";
    const std::string recognizeRegExpr = R"(^recognize ([0-9]+) ([a-z0-9]+)$)";
    const std::string unionRegExpr = R"(^union ([0-9]+) ([0-9]+)$)";
    const std::string concatRegExpr = R"(^concat ([0-9]+) ([0-9]+)$)";
    const std::string unRegExpr = R"(^un ([0-9]+)$)";
    const std::string regExpr = R"(^reg ([a-z0-9*.U\(\)+]+)$)";

    //to get the file name, concatenate capture group 2 with capture group 4

    const std::sregex_iterator end;

    const std::regex openPattern = std::regex(openRegExpr);
    const std::regex listPattern = std::regex(listRegExpr);
    const std::regex printPattern = std::regex(printRegExpr);
    const std::regex savePattern = std::regex(saveRegExpr);
    const std::regex exitPattern = std::regex(exitRegExpr);
    const std::regex emptyPattern = std::regex(emptyRegExpr);
    const std::regex infinitePattern = std::regex(infiniteRegExpr);
    const std::regex deterministicPattern = std::regex(deterministicRegExpr);
    const std::regex determinePattern = std::regex(determineRegExpr);
    const std::regex recognizePattern = std::regex(recognizeRegExpr);
    const std::regex unionPattern = std::regex(unionRegExpr);
    const std::regex concatPattern = std::regex(concatRegExpr);
    const std::regex unPattern = std::regex(unRegExpr);
    const std::regex regPattern = std::regex(regExpr);

    std::sregex_iterator iter;

    std::string text;

    while (true) {
        std::cout << ">";
        getline(std::cin, text);

        if ((iter = std::sregex_iterator(text.begin(), text.end(), exitPattern)) != end) {
            std::cout << "exiting...";
            return;
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), openPattern)) != end) {
            std::string fileName = std::string((*iter)[2]) + std::string((*iter)[4]);
            open(fileName);
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), listPattern)) != end) {
            listNFAs();
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), printPattern)) != end) {
            printNFA(std::stoi((*iter)[1]));
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), savePattern)) != end) {
            std::string fileName = std::string((*iter)[3]) + std::string((*iter)[5]);
            saveNFA(std::stoi((*iter)[1]), fileName);
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), emptyPattern)) != end) {
            emptyNFA(std::stoi((*iter)[1]));
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), infinitePattern)) != end) {
            infiniteNFA(std::stoi((*iter)[1]));
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), deterministicPattern)) != end) {
            deterministicNFA(std::stoi((*iter)[1]));
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), determinePattern)) != end) {
            determineNFA(std::stoi((*iter)[1]));
        }   else if ((iter = std::sregex_iterator(text.begin(), text.end(), recognizePattern)) != end) {
            std::string word = std::string((*iter)[2]);
            recognizeWord(std::stoi((*iter)[1]),word);
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), unionPattern)) != end) {
            unionNFAs(std::stoi((*iter)[1]),std::stoi((*iter)[2]));
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), concatPattern)) != end) {
            concatNFAs(std::stoi((*iter)[1]),std::stoi((*iter)[2]));
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), unPattern)) != end) {
            unNFA(std::stoi((*iter)[1]));
        } else if ((iter = std::sregex_iterator(text.begin(), text.end(), regPattern)) != end){
            reg(std::string((*iter)[1]));
        }else
        {
            std::cout<<"Error : invalid command syntax."<<std::endl;
        }
        std::cout<<std::endl;
    }
}
