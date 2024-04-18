#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "MLayer.h"
#include <fstream>
#include <regex>

const std::regex MLayerRe{"\"MLayer-[0-9]+\""};
const std::regex IdRe{"\"Id\":[\\s\t]*[0-9]+"};
const std::regex ParentIdRe{"\"Parent id\":[\\s\t]*[0-9]+"};
const std::regex ShapeMapRe{"\"Shape maps\""};

int fieldToInt(const std::string& field);
MLayer parseMLayer(std::ifstream& fs, int id);

std::vector<MLayer> parseAllNonEmptyMLayers(std::ifstream& fs);

#endif // UTILS_H
