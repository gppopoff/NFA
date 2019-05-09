//
// Created by Jorko P on 09-May-19.
//

#ifndef NFA_STATE_H
#define NFA_STATE_H

#include "Trans.h"
#include "Array.h"

class State {
    Array<Trans> transitions;
//    Trans *transitions;
//    int numTrans;
    bool isFinal,isInitial;

public:
    State();
//    void setTranss(Trans*);
//    void setNumTrans(int);

    void setTranss(const Array<Trans>&);
    void makeFinal();
    void makeUnfinal();
    void makeInitial();
    void makeUninitial();

    Array<Trans> getTranss() const { return transitions; };
    bool isStateFinal() const { return isFinal; }
    bool isStateInitial() const { return isInitial; }

    void addTrans(Trans);
    void removeTrans(Trans);

    void print() const;
};


#endif //NFA_STATE_H
