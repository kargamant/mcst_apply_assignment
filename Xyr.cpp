#include "Xyr.h"
#include <format>


void Xyr::print(std::ostream& stream)
{
    stream<<std::format("({}, {}, {})", x, y, r)<<std::endl;
}
