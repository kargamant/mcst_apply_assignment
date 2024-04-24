#include <Layer.h>

void Layer::print(std::ostream& stream)
{
    stream<<"Layer-"<<id<<std::endl;
    stream<<"sublayers ids: ";
    std::copy(subLayers.begin(), subLayers.end(), std::ostream_iterator<int>(stream, " "));
    stream<<std::endl;
}
