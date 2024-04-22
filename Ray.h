#ifndef RAY_H
#define RAY_H
#include "Xyr.h"
#include <iostream>
#include "Shape.h"

enum class rayType
{
    Vertical,
    Horizontal
};

struct Ray{
    rayType type;
    int level;
    int coordinate;
    std::pair<Xyr, Xyr> dotPair;
    Ray(rayType type=rayType::Horizontal, int level=0, int coordinate=0, std::pair<int, int> dotPair={0, 0}) : type(type), level(level), coordinate(coordinate), dotPair(dotPair) {}
    //Ray(Circle circle);

    void print(std::ostream& stream=std::cout) const;
    bool operator<(const Ray& ray2) const { return type == ray2.type ? level < ray2.level : throw("different types"); }
};

#endif // RAY_H
