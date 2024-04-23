#include "traversing.h"
#include <regex>
#include <algorithm>
#include "Layer.h"
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

std::vector<Xyr> traverseFromTo(rayType direction, bool forward, std::vector<Xyr>& polyVert, std::vector<Xyr>& circles)
{
    std::vector<Xyr> result;
    /*
    direction==Horizontal && forward - from left to right
    direction==Horizontal && !forward - from right to left
    direction==Vertical && forward - from down to up
    direction==Vertical && !forward - from up to down
    */

    if(direction==rayType::Horizontal && forward) std::sort(polyVert.begin(), polyVert.end(), [](Xyr a, Xyr b) {return a.x < b.x; });
    else if(direction==rayType::Horizontal && !forward) std::sort(polyVert.begin(), polyVert.end(), [](Xyr a, Xyr b) {return a.x > b.x; });
    else if(direction==rayType::Vertical && forward) std::sort(polyVert.begin(), polyVert.end(), [](Xyr a, Xyr b) {return a.y < b.y; });
    else std::sort(polyVert.begin(), polyVert.end(), [](Xyr a, Xyr b) {return a.y > b.y; });

    std::unordered_map<int, std::vector<Xyr>> x_groups;
    std::unordered_map<int, std::vector<Xyr>> y_groups;
    if (direction == rayType::Horizontal && forward) x_groups = doubleSortX(forward, polyVert, circles);
    else if (direction == rayType::Horizontal && !forward) x_groups = doubleSortX(!forward, polyVert, circles);
    else if (direction == rayType::Vertical && forward) y_groups = doubleSortY(forward, polyVert, circles);
    else y_groups = doubleSortY(!forward, polyVert, circles);


    if (direction == rayType::Horizontal && forward) std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.x < b.x; });
    else if (direction == rayType::Horizontal && !forward) std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.x > b.x; });
    else if (direction == rayType::Vertical && forward) std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.y < b.y; });
    else std::sort(circles.begin(), circles.end(), [](Xyr a, Xyr b) {return a.y > b.y; });

    std::set<Ray> polyHorizontalRays;
    std::set<Ray> polyVerticalRays;
    Ray minVertRay{ rayType::Vertical, std::numeric_limits<int>::max() };
    Ray maxVertRay{ rayType::Vertical, std::numeric_limits<int>::min() };
    Ray minHorRay{ rayType::Horizontal, std::numeric_limits<int>::max() };
    Ray maxHorRay{ rayType::Horizontal, std::numeric_limits<int>::min() };

    for (auto& vert : polyVert)
    {
        if (!polyHorizontalRays.contains(Ray(rayType::Horizontal, vert.y, vert.x)))
        {
            if (minHorRay.level > vert.y)
            {
                minHorRay.level = vert.y;
                minHorRay.coordinate = vert.x;
            }
            if (maxHorRay.level < vert.y)
            {
                maxHorRay.level = vert.y;
                maxHorRay.coordinate = vert.x;
            }
            polyHorizontalRays.insert(Ray(rayType::Horizontal, vert.y, vert.x));
        }
        if (!polyVerticalRays.contains(Ray(rayType::Vertical, vert.x, vert.y)))
        {
            if (minVertRay.level > vert.x)
            {
                minVertRay.level = vert.x;
                minVertRay.coordinate = vert.y;
            }
            if (maxVertRay.level < vert.x)
            {
                maxVertRay.level = vert.x;
                maxVertRay.coordinate = vert.y;
            }
            polyVerticalRays.insert(Ray(rayType::Vertical, vert.x, vert.y));
        }
    }

    std::cout << "debug" << std::endl;
    maxVertRay.print();
    minVertRay.print();
    maxHorRay.print();
    minHorRay.print();
    if (direction == rayType::Vertical && !forward)
    {
        std::cout << "different debug" << std::endl;
        y_groups[polyVert[0].y][0].print();
    }
    else if (direction == rayType::Horizontal && !forward)
    {
        std::cout << "different debug2" << std::endl;
        x_groups[polyVert.back().x].back().print();
    }
    else if (direction == rayType::Vertical && forward)
    {
        std::cout << "different debug3" << std::endl;
        y_groups[polyVert[0].y].back().print();

    }
    
    /*if (!forward)
    {
        std::reverse(polyHorizontalRays.begin(), polyHorizontalRays.end());
        std::reverse(polyVerticalRays.begin(), polyVerticalRays.end());
    }*/

    if (direction == rayType::Horizontal && forward)
    {
        result.push_back(x_groups[minVertRay.level][0]);
        int border_y = x_groups[minVertRay.level][0].y;
        int x = minVertRay.level;
        Ray border(rayType::Horizontal, border_y, x);
        //std::cout << "x y: " << x << y << std::endl;
        for (auto& circle : circles)
        {
            if (circle.checked) continue;
            int y = circle.y;
            bool skip = false;
            for (auto& polyRay : polyHorizontalRays)
            {
                if (circle.y < polyRay.level && polyRay.level < border_y)
                {
                    skip = true;
                    break;
                }
            }
            //std::cout << skip << std::endl;
            if (skip == true) continue;
            else
            {
                std::cout << "circle to traverse:";
                circle.print();
                std::vector<Xyr> circle_path = formPathToCircle(circle, border);
                circle.checked = true;
                std::move(circle_path.begin(), circle_path.end(), std::back_inserter<std::vector<Xyr>>(result));
            }
        }
    }
    else if (direction == rayType::Vertical && !forward)
    {
            result.push_back(y_groups[polyVert[0].y][0]);
            int border_x = y_groups[polyVert[0].y][0].x;
            int y = maxVertRay.level;
            Ray border(rayType::Vertical, border_x, y);
            //std::cout << "x y: " << x << y << std::endl;
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
                //std::cout << skip << std::endl;
                if (skip == true) continue;
                else
                {
                    std::cout << "circle to traverse:";
                    circle.print();
                    std::vector<Xyr> circle_path = formPathToCircle(circle, border);
                    circle.checked = true;
                    std::move(circle_path.begin(), circle_path.end(), std::back_inserter<std::vector<Xyr>>(result));
                }
            }
        
    }
    else if (direction == rayType::Horizontal && !forward)
    {
        result.push_back(x_groups[polyVert.back().x].back());
        int border_y = x_groups[polyVert.back().x].back().y;
        int x = maxVertRay.level;
        Ray border(rayType::Horizontal, border_y, x);
        //std::cout << "x y: " << x << y << std::endl;
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
            //std::cout << skip << std::endl;
            if (skip == true) continue;
            else
            {
                std::cout << "circle to traverse:";
                circle.print();
                std::vector<Xyr> circle_path = formPathToCircle(circle, border);
                circle.checked = true;
                std::move(circle_path.begin(), circle_path.end(), std::back_inserter<std::vector<Xyr>>(result));
            }
        }
    }
    else
    {
        result.push_back(y_groups[polyVert[0].y].back());
        int border_x = y_groups[polyVert[0].y].back().x;
        int y = minVertRay.level;
        Ray border(rayType::Vertical, border_x, y);
        //std::cout << "x y: " << x << y << std::endl;
        for (auto& circle : circles)
        {
            if (circle.checked) continue;
            int x = circle.x;
            bool skip = false;
            for (auto& polyRay : polyVerticalRays)
            {
                if (circle.x >= polyRay.level && polyRay.level < border_x)
                {
                    skip = true;
                    break;
                }
            }
            //std::cout << skip << std::endl;
            if (skip == true) continue;
            else
            {
                std::cout << "circle to traverse:";
                circle.print();
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