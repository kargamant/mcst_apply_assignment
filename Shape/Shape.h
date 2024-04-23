#ifndef SHAPE_H
#define SHAPE_H
#include <utility>
#include <vector>
#include <string>
#include <iostream>

enum class ShapeType
{
    Polygon,
    Circle,
    Region
};

std::string shapeTypeToString(ShapeType type);

struct Shape{
    int id=0;
    ShapeType type;
    int bm=0;
    int lt=0;
    int rt=0;
    int tp=0;
    Shape() : type(ShapeType::Polygon) {}

    virtual void print(std::ostream& stream=std::cout);
};

struct Circle: public Shape
{
    int radius;
    std::pair<int, int> coords;
    Circle() : Shape() {type=ShapeType::Circle;}

    void print(std::ostream& stream=std::cout) override;
};

struct Vertex
{
    int id;
    std::pair<int, int> coords;
    void printV(std::ostream& stream=std::cout);
};

//can be either region or pure polygon
struct Polygon : public Shape
{
    std::vector<Vertex> vert;
    Polygon() : Shape() { type=ShapeType::Polygon;}
    void print(std::ostream& stream=std::cout) override;
};

#endif // SHAPE_H
