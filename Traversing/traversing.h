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
#include <functional>

std::vector<Xyr> formPathToCircle(const Xyr& circle, const Ray& ray);
std::vector<Xyr> traverseFromTo(Xyr& from, Xyr& to, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles, std::set<Ray>& polyHorizontalRays, std::set<Ray>& polyVerticalRays);
void visitCircles(rayType type, Xyr& from, std::vector<Xyr>& circles, const std::set<Ray>& polyRays, std::function<bool(Xyr&, Ray, Ray&)> criteria, std::vector<Xyr>& result);

#endif // UTILS_H
