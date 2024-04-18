#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "MLayer.h"
#include <fstream>
#include <regex>
#include "Shape.h"

const std::regex MLayerRe{"\"MLayer-[0-9]+\""};
const std::regex IdRe{"\"Id\":[\\s\t]*[0-9]+"};
const std::regex ParentIdRe{"\"Parent id\":[\\s\t]*[0-9]+"};
const std::regex ShapeMapRe{"\"Shape maps\""};
const std::regex bmRe{"\"bm\":[\\s\t]*-?[0-9]+"};
const std::regex ltRe{"\"lt\":[\\s\t]*-?[0-9]+"};
const std::regex rtRe{"\"rt\":[\\s\t]*-?[0-9]+"};
const std::regex tpRe{"\"tp\":[\\s\t]*-?[0-9]+"};
const std::regex diameterRe{"\"Diameter\":[\\s\t]*[0-9]+"};
const std::regex xRe{"\"X\":[\\s\t]*-?[0-9]+"};
const std::regex yRe{"\"X\":[\\s\t]*-?[0-9]+"};

int fieldToInt(const std::string& field);
MLayer parseMLayer(std::ifstream& fs, int id);

std::vector<MLayer> parseAllNonEmptyMLayers(std::ifstream& fs);

Shape* parseShape(std::ifstream& fs, int shape_id, ShapeType shape_type);
#endif // UTILS_H
