#include <Ray.h>


void Ray::print(std::ostream& stream) const
{
    if(type==rayType::Horizontal) stream<<"Horizontal-";
    if(type==rayType::Vertical) stream<<"Vertical-";
    stream<<level<<std::endl;
    stream << "coordinate: " << coordinate << std::endl;
}
