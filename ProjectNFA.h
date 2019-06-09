//
// Created by Jorko P on 08-Jun-19.
//

#ifndef NFA_PROJECTNFA_H
#define NFA_PROJECTNFA_H

#include "NFA.h"
#include <vector>
#include <string>


class ProjectNFA {
    std::vector<NFA> list;

public:
    void open(const std::string&);
    void listNFAs()const;
    void printNFA(int id)const ;
    void saveNFA(int id,const std::string&)const ;
    void emptyNFA(int id) const;
    void deterministicNFA(int id)const;
    void recognizeWord(int id,const std::string&) const;
    void unionNFAs(int,int);
    void concatNFAs(int,int);
    void unNFA(int);
    void reg(const std::string&);
    void infiniteNFA(int id)const;
    void Activate();

};


#endif //NFA_PROJECTNFA_H
