#ifndef TRAVERSING_H
#define TRAVERSING_H
#include <string>
#include <MLayer.h>
#include <fstream>
#include <regex>
#include <Shape.h>
#include <Layer.h>
#include <unordered_map>
#include <Xyr.h>
#include <Ray.h>
#include <set>

std::vector<Xyr> formPathToCircle(const Xyr& circle, const Ray& ray);
std::vector<Xyr> traverseFromTo(Xyr& from, Xyr& to, bool forward, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles, std::set<Ray>& polyHorizontalRays, std::set<Ray>& polyVerticalRays);
std::unordered_map<int, std::vector<Xyr>> doubleSortX(bool forward, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles);
std::unordered_map<int, std::vector<Xyr>> doubleSortY(bool forward, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles);

#endif // UTILS_H
