#ifndef LAYER_H
#define LAYER_H
#include <vector>
#include <iostream>

struct Layer
{
    int id;
    std::vector<int> subLayers;
    void print(std::ostream& stream=std::cout);
};

#endif // LAYER_H
