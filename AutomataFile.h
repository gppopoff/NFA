//
// Created by Jorko P on 31-May-19.
//

#ifndef NFA_AUTOMATAFILE_H
#define NFA_AUTOMATAFILE_H

#include "NFA.h"
#include <fstream>

class AutomataFile {
    NFA automata;
    std::string fileName;
    bool fileOpened;
public:
    void open(const std::string&);


};


#endif //NFA_AUTOMATAFILE_H
