#include "Shape.h"

std::string shapeTypeToString(ShapeType type)
{
    switch(type)
    {
    case ShapeType::Circle:
        return "Circle";
    case ShapeType::Polygon:
        return "Polygon";
    case ShapeType::Region:
        return "Region";
    }
}

void Shape::print(std::ostream& stream)
{
    stream<<shapeTypeToString(type)<<"-"<<id<<std::endl;
    stream<<"bm: "<<bm<<std::endl;
    stream<<"lt: "<<lt<<std::endl;
    stream<<"rt: "<<rt<<std::endl;
    stream<<"tp: "<<tp<<std::endl;
}

void Circle::print(std::ostream& stream)
{
    Shape sh=*this;
    sh.print();
    stream<<"radius: "<<radius<<std::endl;
    stream<<"X: "<<coords.first<<std::endl;
    stream<<"Y: "<<coords.second<<std::endl;
}

void Vertex::printV(std::ostream& stream)
{
    stream<<"Vertex-"<<id<<std::endl;
    stream<<"X: "<<coords.first<<std::endl;
    stream<<"Y: "<<coords.second<<std::endl;
}

void Polygon::print(std::ostream& stream)
{
    Shape sh=*this;
    sh.print();
    for(auto& v: vert) v.printV(stream);
}
