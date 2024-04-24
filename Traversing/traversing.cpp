#include "traversing.h"
#include <regex>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
#include <limits>
#include <functional>

std::vector<Xyr> formPathToCircle(const Xyr& circle, const Ray& ray)
{
    std::vector<Xyr> result;
    if (ray.type == rayType::Horizontal)
    {
        result.emplace_back(circle.x - circle.r, ray.level);
        result.emplace_back(circle.x - circle.r, circle.y);
        result.emplace_back(circle.x, circle.y, -circle.r);
        result.emplace_back(circle.x - circle.r, circle.y);
        result.emplace_back(circle.x - circle.r, ray.level);
    }
    else
    {
        result.emplace_back(ray.level, circle.y + circle.r);
        result.emplace_back(circle.x, circle.y + circle.r);
        result.emplace_back(circle.x, circle.y, -circle.r);
        result.emplace_back(circle.x, circle.y + circle.r);
        result.emplace_back(ray.level, circle.y + circle.r);
        
    }

    
    return result;
}

std::vector<Xyr> traverseFromTo(Xyr& from, Xyr& to, bool forward, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles, std::set<Ray>& polyHorizontalRays, std::set<Ray>& polyVerticalRays)
{
    //First define orientation of current move
    rayType direction;
    if (from.x == to.x) direction = rayType::Vertical;
    else if (from.y == to.y) direction = rayType::Horizontal;
    else return { from }; //if it is diagonal border then skip it

    std::vector<Xyr> result;
    /*
    This is how direction is defined

    direction==Horizontal && forward - from left to right
    direction==Horizontal && !forward - from right to left
    direction==Vertical && forward - from down to up
    direction==Vertical && !forward - from up to down
    */

    //Sorting circles in a way they will be traversed in this move
    if (direction == rayType::Horizontal && forward) std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.x < b.x; });
    else if (direction == rayType::Horizontal && !forward) std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.x > b.x; });
    else if (direction == rayType::Vertical && forward) std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.y < b.y; });
    else std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.y > b.y; });


    //Rays are basically horizontal and vertical lines that goes through a polygon vertecies
    //If given sets of this rays are empty we need to form them again
    if (polyHorizontalRays.empty() || polyVerticalRays.empty())
    {
        for (auto& vert : polyVert)
        {
            if (!polyHorizontalRays.contains(Ray(rayType::Horizontal, vert.y, vert.x)))
            {
                polyHorizontalRays.insert(Ray(rayType::Horizontal, vert.y, vert.x));
            }
            if (!polyVerticalRays.contains(Ray(rayType::Vertical, vert.x, vert.y)))
            {
                polyVerticalRays.insert(Ray(rayType::Vertical, vert.x, vert.y));
            }
        }
    }
    
    std::function<bool(Xyr&, Ray, Ray&)> criteria;
    //traversing itself
    if (direction == rayType::Horizontal && forward)
    {
        criteria = [](Xyr& circle, Ray polyRay, Ray& border) {return circle.y < polyRay.level && polyRay.level < border.level; };
    }
    else if (direction == rayType::Vertical && !forward)
    {
        criteria = [](Xyr& circle, Ray polyRay, Ray& border) {return circle.x < polyRay.level&& polyRay.level < border.level; };
    }
    else if (direction == rayType::Horizontal && !forward)
    {
        criteria= [](Xyr& circle, Ray polyRay, Ray& border) {return circle.y >= polyRay.level && polyRay.level > border.level; };
    }
    else
    {
        criteria= [](Xyr& circle, Ray polyRay, Ray& border) {return circle.x >= polyRay.level && polyRay.level > border.level; };
    }
    
    if(direction==rayType::Horizontal) visitCircles(direction, from, circles, polyHorizontalRays, criteria, result);
    else visitCircles(direction, from, circles, polyVerticalRays, criteria, result);

    return result;
}

void visitCircles(rayType type, Xyr& from, std::vector<Xyr>& circles, const std::set<Ray>& polyRays, std::function<bool(Xyr&, Ray, Ray&)> criteria, std::vector<Xyr>& result)
{
    result.push_back(from);

    //border to connect to
    Ray border(type, from.x, from.y);
    if (type == rayType::Horizontal) std::swap(border.level, border.coordinate);

    for (auto& circle : circles)
    {
        //if  it has already been connected by some other border
        if (circle.checked) continue;
        bool skip = false;
        for (auto& polyRay : polyRays)
        {
            // Example for case Horizontal forward
            // 
            // if a circle is under some polyRay and this polyRay is under border, then it is unreachable and should be skipped
                //example
                /*
                    +------------+  //border

                    +------------+  //polyRay
                        o  o        //unreachable circles
                        o

                */
            if (criteria(circle, polyRay, border))
            {
                skip = true;
                break;
            }
        }
        if (skip == true) continue;
        else
        {
            std::vector<Xyr> circle_path = formPathToCircle(circle, border);
            circle.checked = true;
            std::move(circle_path.begin(), circle_path.end(), std::back_inserter<std::vector<Xyr>>(result));
        }
    }
}
