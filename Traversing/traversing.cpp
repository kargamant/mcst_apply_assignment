#include "traversing.h"
#include <regex>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
#include <limits>

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
    
    //traversing itself
    if (direction == rayType::Horizontal && forward)
    {
        result.push_back(from);

        //border to connect to
        int border_y = from.y;
        int x = from.x;
        Ray border(rayType::Horizontal, border_y, x);
        
        
        for (auto& circle : circles)
        {
            //if  it has already been connected by some other border
            if (circle.checked) continue;
            
            int y = circle.y;
            bool skip = false;
            for (auto& polyRay : polyHorizontalRays)
            {
                //if a circle is under some border(ray) and this border is under border_y, then it is unreachable and should be skipped
                //example
                /*
                    +------------+  //border_y

                    +------------+  //polyRay
                        o  o        //unreachable circles
                        o

                */
                if (circle.y < polyRay.level && polyRay.level < border_y)
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
    else if (direction == rayType::Vertical && !forward)
    {
            result.push_back(from);
            int border_x = from.x;
            int y = from.y;
            Ray border(rayType::Vertical, border_x, y);
            for (auto& circle : circles)
            {
                if (circle.checked) continue;
                int x = circle.x;
                bool skip = false;
                for (auto& polyRay : polyVerticalRays)
                {
                    if (circle.x < polyRay.level && polyRay.level < border_x)
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
    else if (direction == rayType::Horizontal && !forward)
    {
        result.push_back(from);
        int border_y = from.y;
        int x = from.x;
        Ray border(rayType::Horizontal, border_y, x);
        for (auto& circle : circles)
        {
            if (circle.checked) continue;
            int y = circle.y;
            bool skip = false;
            for (auto& polyRay : polyHorizontalRays)
            {
                if (circle.y >= polyRay.level && polyRay.level > border_y)
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
    else
    {
        result.push_back(from);
        int border_x = from.x;
        int y = from.y;
        Ray border(rayType::Vertical, border_x, y);
        for (auto& circle : circles)
        {
            if (circle.checked) continue;
            int x = circle.x;
            bool skip = false;
            for (auto& polyRay : polyVerticalRays)
            {
                std::cout << "debuuuug" << std::endl;
                polyRay.print();
                circle.print();
                std::cout << border_x << std::endl;
                if (circle.x >= polyRay.level && polyRay.level > border_x)
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
    return result;
}

std::unordered_map<int, std::vector<Xyr>> doubleSortX(bool forward, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles)
{
    //soring vertecies with same x by y
    int x = polyVert[0].x;
    int prev = 0;
    std::vector<Xyr> same_x;
    std::unordered_map<int, std::vector<Xyr>> x_groups;
    same_x.push_back(polyVert[0]);
    for (int i = 1; i < polyVert.size(); i++)
    {
        if (polyVert[i].x == x)
        {
            same_x.push_back(polyVert[i]);
        }
        else
        {
            if(forward) std::sort(same_x.begin(), same_x.end(), [](Xyr a, Xyr b) {return a.y > b.y; });
            else std::sort(same_x.begin(), same_x.end(), [](Xyr a, Xyr b) {return a.y < b.y; });
            std::copy(same_x.begin(), same_x.end(), polyVert.begin() + prev);
            x_groups.insert({ x, same_x });
            x = polyVert[i].x;
            prev = i;
            same_x.clear();
            same_x.push_back(polyVert[i]);
        }
    }
    if (forward) std::sort(same_x.begin(), same_x.end(), [](Xyr a, Xyr b) {return a.y > b.y; });
    else std::sort(same_x.begin(), same_x.end(), [](Xyr a, Xyr b) {return a.y < b.y; });
    std::copy(same_x.begin(), same_x.end(), polyVert.begin() + prev);
    x_groups.insert({ x, same_x });
    
    return x_groups;
}

std::unordered_map<int, std::vector<Xyr>> doubleSortY(bool forward, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles)
{
    //soring vertecies with same y by x
    int y = polyVert[0].y;
    int prev = 0;
    std::vector<Xyr> same_y;
    std::unordered_map<int, std::vector<Xyr>> y_groups;
    same_y.push_back(polyVert[0]);
    for (int i = 1; i < polyVert.size(); i++)
    {
        if (polyVert[i].y == y)
        {
            same_y.push_back(polyVert[i]);
        }
        else
        {
            if (forward) std::sort(same_y.begin(), same_y.end(), [](Xyr a, Xyr b) {return a.x > b.x; });
            else std::sort(same_y.begin(), same_y.end(), [](Xyr a, Xyr b) {return a.x < b.x; });
            std::copy(same_y.begin(), same_y.end(), polyVert.begin() + prev);
            y_groups.insert({ y, same_y });
            y = polyVert[i].y;
            prev = i;
            same_y.clear();
            same_y.push_back(polyVert[i]);
        }
    }
    if (forward) std::sort(same_y.begin(), same_y.end(), [](Xyr a, Xyr b) {return a.x > b.x; });
    else std::sort(same_y.begin(), same_y.end(), [](Xyr a, Xyr b) {return a.x < b.x; });
    std::copy(same_y.begin(), same_y.end(), polyVert.begin() + prev);
    y_groups.insert({ y, same_y });

    return y_groups;
}