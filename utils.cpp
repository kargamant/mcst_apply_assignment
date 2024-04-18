#include "utils.h"


int fieldToInt(const std::string& field)
{
    return std::stoi(field.substr(field.find(":")+1, field.length()));
}
