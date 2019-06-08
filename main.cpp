#include <iostream>
#include <algorithm>
#include "Trans.h"
#include "State.h"
#include "NFA.h"
#include "filefunctions.h"
#include "ProjectNFA.h"

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

    Trans t1('E',1),t2('a',2),t3('b',3),t4('E',3),t5('b',0),t6('E',0),t7('E',2);
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
    NFA n,n2;

    n.addState(s1);
    n.addState(s2);
    n.addState(s3);
    n.addState(s4);
    n.detemine().print();
*/

//    NFA n5 = n.detemine();
//    n5.print();
//
//            n2.addState(s1);
//            n2.addState(s2);
//            n2.addState(s3);
//            n2.addState(s4);
//            NFA n3 = n.concat(n2);
//            n3.print();
//            NFA n4 = n.un();
//            n4.print();

    /*    std::cout<<n.recognize("aaaa")<<std::endl;
    std::cout<<n.recognize("aabababba")<<std::endl;
    std::cout<<n.recognize("aab")<<std::endl;
    std::cout<<n.recognize("b")<<std::endl;
    std::cout<<n.recognize("aabababbbaaaa")<<std::endl;
    std::cout<<n.recognize("aaaba$")<<std::endl;
    std::cout<<n.recognize("")<<std::endl;
    std::cout<<n.recognize("aab")<<std::endl;*/ // тестове за recognize
     /*Trans t1('E',1),t2('0',1),t3('0',2),t4('1',0),t5('1',1),t6('E',2),t7('0',2),t8('1',2);
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
    n2.detemine();*/
 //1 автомат
/*
    Trans t1('a',1),t2('a',2),t3('b',1),t4('b',3);
    State s0,s1,s2,s3;
    s0.addTrans(t1);
    s0.addTrans(t2);
    s1.addTrans(t1);
    s1.addTrans(t2);
    s2.addTrans(t3);
    s3.addTrans(t1);
    s3.addTrans(t2);
    s0.makeInitial();
    s3.makeFinal();

    NFA test;
    test.addState(s0);
    test.addState(s1);
    test.addState(s2);
    test.addState(s3);
    std::cout<<test.isLanguageEmpty()<<"\n";
    std::cout<<test.getId()<<"\n";
    NFA jojo('a');
    test.unite(NFA()).print();
*/

/*
    std::string b = "(ab)*((ab)+c*)";
//    std::string b = "ab*+c";
//    std::string b = "a+b+c";
    NFA a(b);
    a.print();
    a.detemine().print();
*/

    ProjectNFA a;
    a.open("automaton.txt");
    a.listNFAs();
    a.printNFA(1);
    a.unNFA(1);
    a.printNFA(2);
    a.saveNFA(2,"automaton2.txt");
    a.reg("(ab)*((ab)+c*)");
    a.printNFA(4);

    return 0;
}