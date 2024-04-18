#include "MLayer.h"


void MLayer::print(std::ostream& stream)
{
    stream<<"MLayer-"<<id<<std::endl;
    stream<<"Parent id: "<<parentId<<std::endl;
    stream<<"shapes map ids: ";
    std::copy(shapeMap.begin(), shapeMap.end(), std::ostream_iterator<int>(stream, " "));
    stream<<std::endl;
}
