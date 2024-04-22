#include "Xyr.h"
#include <format>


void Xyr::print(std::ostream& stream)
{
    stream<<std::format("({}, {}, {})", x, y, r)<<std::endl;
}

/*bool compareByX(const Xyr& xyr1, const Xyr& xyr2)
{
    return xyr1.x < xyr2.x;
}
bool compareByY(const Xyr& xyr1, const Xyr& xyr2)
{
    return xyr1.y < xyr2.y;
}*/
