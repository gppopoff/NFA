//
// Created by Jorko P on 09-May-19.
//

#ifndef NFA_TRANS_H
#define NFA_TRANS_H


class Trans {
    char symbol;
    int path;

public:
    Trans(char s = 'E', int p = -1);
    void setSymbol(char s);
    void setPath(int p);
    char getSymbol() const { return symbol; }
    char getPath() const { return path; }
    void print() const;

    bool operator==(const Trans&) const;

};

#endif //NFA_TRANS_H
