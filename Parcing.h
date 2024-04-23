#ifndef PARSE_H
#define PARSE_H
#include <string>
#include "MLayer.h"
#include <fstream>
#include <regex>
#include "Shape.h"
#include "Layer.h"
#include <unordered_map>
#include "Xyr.h"
#include "Ray.h"

const std::regex MLayerRe{ "\"MLayer-[0-9]+\"" };
const std::regex LayerRe{ "\"Layer-[0-9]+\"" };
const std::regex IdRe{ "\"Id\":[\\s\t]*[0-9]+" };
const std::regex ParentIdRe{ "\"Parent id\":[\\s\t]*[0-9]+" };
const std::regex ShapeMapRe{ "\"Shape maps\"" };
const std::regex bmRe{ "\"bm\":[\\s\t]*-?[0-9]+" };
const std::regex ltRe{ "\"lt\":[\\s\t]*-?[0-9]+" };
const std::regex rtRe{ "\"rt\":[\\s\t]*-?[0-9]+" };
const std::regex tpRe{ "\"tp\":[\\s\t]*-?[0-9]+" };
const std::regex diameterRe{ "\"Diameter\":[\\s\t]*[0-9]+" };
const std::regex xRe{ "\"X\":[\\s\t]*-?[0-9]+" };
const std::regex yRe{ "\"Y\":[\\s\t]*-?[0-9]+" };
const std::regex vertexCountRe{ "\"Vertex count\":[\\s\t]*[0-9]+" };
const std::regex vertexIdRe{ "\"Vertex-[0-9]+\"" };
const std::regex layerNumRe{ "\"Layer Num\":[\\s\t]*[0-9]+" };


int fieldToInt(const std::string& field, const std::string& delimeter = ":");
MLayer parseMLayer(std::ifstream& fs, int id);

std::unordered_map<int, MLayer> parseAllNonEmptyMLayers(std::ifstream& fs);

Shape* parseShape(std::ifstream& fs, int shape_id, ShapeType shape_type);
Layer parseLayer(std::ifstream& fs, int layer_id);

void parseAny(std::ifstream& fs, std::vector<std::string>& results, const std::vector<std::regex>& reg);

void drop_results(const std::vector<Xyr>& result, std::ofstream& fs);

#endif