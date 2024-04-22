#ifndef XYR_H
#define XYR_H
#include <iostream>

struct Xyr
{
    int x=0;
    int y=0;
    int r=0;
    bool checked = false;
    Xyr(int x=0, int y=0, int r=0) : x(x), y(y), r(r) {}
    void print(std::ostream& stream=std::cout);
};

//bool compareByX(const Xyr& xyr1, const Xyr& xyr2);
//bool compareByY(const Xyr& xyr1, const Xyr& xyr2);


#endif // XYR_H
