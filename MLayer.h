#ifndef MLAYER_H
#define MLAYER_H
#include <vector>
#include <iostream>

struct MLayer
{
    int id=0;
    int parentId=0;
    std::vector<int> shapeMap;
    void print(std::ostream& stream=std::cout);
};

#endif // MLAYER_H
