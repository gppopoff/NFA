#include <iostream>
#include <algorithm>
#include "Trans.h"
#include "State.h"
#include "NFA.h"

int main() {

    /*Trans t1('a',1),t2('E',2),t3('b',2),t4('E',1),t5('E',2);
    State s1,s2,s3;
    s1.addTrans(t1);
    s1.addTrans(t2);
    s2.addTrans(t3);
    s2.addTrans(t5);
    s3.addTrans(t4);
    s1.print();
    s2.print();
    s3.print();
    NFA gyz;
    gyz.addState(s1);
    gyz.addState(s2);
    gyz.addState(s3);
    gyz.print();
    std::string epsi,epsi2,epsi3;
    gyz.epsilonclosure(epsi,0);
   // std::sort(epsi.begin(),epsi.end());
    std::cout<<epsi;
    //gyz.epsilonclosure(epsi2,2);
    std::cout<<epsi2;
   // std::cout<<epsi[1] - '0';*/ //нещо
    /*
    Trans t1('E',1),t2('a',2),t3('b',3),t4('E',3),t5('b',0),t6('E',0);
    State s1,s2,s3,s4;
    s1.addTrans(t1);
    s1.makeInitial();
    s2.addTrans(t2);
    s2.makeInitial();
    s3.addTrans(t3);
    s3.addTrans(t4);
    s4.addTrans(t5);
    s4.addTrans(t6);
    s4.makeFinal();
    NFA n;
    n.addState(s1);
    n.addState(s2);
    n.addState(s3);
    n.addState(s4);
    n.print();
    n.detemine();
*/ // 1 автомат

    Trans t1('E',1),t2('0',1),t3('0',2),t4('1',0),t5('1',1),t6('E',2),t7('0',2),t8('1',2);
    State s1,s2,s3;
    s1.addTrans(t1);
    s1.addTrans(t2);
    s1.addTrans(t3);
    s1.addTrans(t4);
    s2.addTrans(t5);
    s2.addTrans(t6);
    s3.addTrans(t7);
    s3.addTrans(t8);
    s1.makeInitial();
    s3.makeFinal();
    NFA n2;
    n2.addState(s1);
    n2.addState(s2);
    n2.addState(s3);
    n2.detemine();

    return 0;
}