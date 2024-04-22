#include "utils.h"
#include <regex>
#include <algorithm>
#include "Layer.h"
#include <vector>
#include <unordered_map>
#include <set>
#include <limits>

int fieldToInt(const std::string& field, const std::string& delimeter)
{
    return std::stoi(field.substr(field.find(delimeter)+1, field.length()));
}

bool isEmpty(std::string& str) {return str=="";}

void parseAny(std::ifstream& fs, std::vector<std::string>& results, const std::vector<std::regex>& reg)
{
    std::string bm;
    std::string lt;
    std::string rt;
    std::string tp;
    std::string line;
    while(std::any_of(results.begin(), results.end(), isEmpty))
    {
        std::vector<std::smatch> matchers{results.size()};
        std::getline(fs, line);

        for(int i=0; i<matchers.size(); i++)
        {
            std::regex_search(line, matchers[i], reg[i]);
            if(results[i]=="") results[i]=matchers[i].str();
        }
    }
}

MLayer parseMLayer(std::ifstream& fs, int mlayer_id)
{


    std::string line;
    while (std::getline(fs, line)) {
        std::smatch matchMlayer;
        std::regex_search(line, matchMlayer, MLayerRe);
        if(matchMlayer.str()!="")
        {
            MLayer mlayer;
            //std::cout<<matchMlayer.str()<<std::endl;
            std::vector<std::string> ids{2};
            std::vector<std::regex> reg{IdRe, ParentIdRe};
            parseAny(fs, ids, reg);
            if(fieldToInt(ids[0])!=mlayer_id) continue;
            //std::cout<<fieldToInt(id)<<std::endl;
            //std::cout<<fieldToInt(parent_id)<<std::endl;
            mlayer.id=fieldToInt(ids[0]);
            mlayer.parentId=fieldToInt(ids[1]);
            while(!line.contains("Text map"))
            {
                std::smatch shape_id_matcher;
                std::getline(fs, line);
                std::regex_search(line, shape_id_matcher, IdRe);
                if(shape_id_matcher.str()!="") mlayer.shapeMap.push_back(fieldToInt(shape_id_matcher.str()));
            }
            return mlayer;
        }
    }
    return MLayer();
}

std::unordered_map<int, MLayer> parseAllNonEmptyMLayers(std::ifstream& fs)
{
    std::unordered_map<int, MLayer> result;
    std::string line;
    while (std::getline(fs, line)) {
        std::smatch matchMlayer;
        std::regex_search(line, matchMlayer, MLayerRe);
        if(matchMlayer.str()!="")
        {
            MLayer mlayer;
            //std::cout<<matchMlayer.str()<<std::endl;
            std::vector<std::string> ids{2};
            std::vector<std::regex> reg{IdRe, ParentIdRe};
            parseAny(fs, ids, reg);
            //std::cout<<fieldToInt(id)<<std::endl;
            //std::cout<<fieldToInt(parent_id)<<std::endl;
            mlayer.id=fieldToInt(ids[0]);
            mlayer.parentId=fieldToInt(ids[1]);
            while(!line.contains("Text map"))
            {
                std::smatch shape_id_matcher;
                std::getline(fs, line);
                std::regex_search(line, shape_id_matcher, IdRe);
                if(shape_id_matcher.str()!="") mlayer.shapeMap.push_back(fieldToInt(shape_id_matcher.str()));
            }
            if(mlayer.shapeMap.size()!=0) result.insert({mlayer.id, mlayer});
        }
    }
    fs.close();
    return result;
}


Shape* parseShape(std::ifstream& fs, int shape_id, ShapeType shape_type)
{
    Shape* result;
    switch(shape_type)
    {
    case ShapeType::Circle:
        result=new Circle();
        break;
    case ShapeType::Polygon:
        result=new Polygon();
        break;
    case ShapeType::Region:
        result=new Polygon();
        result->type=ShapeType::Region;
        break;
    }

    std::regex shapeRe{"\""+shapeTypeToString(shape_type)+"-"+std::to_string(shape_id)+"\""};
    std::string line;
    std::smatch shape_matcher;
    while (std::getline(fs, line)) {
        std::regex_search(line, shape_matcher, shapeRe);
        if(shape_matcher.str()!="") break;
    }
    if(shape_matcher.str()=="")
    {
        delete result;
        return result;
    }

    if(shape_type==ShapeType::Circle)
    {
        std::vector<std::string> diameter{1};
        std::vector<std::regex> reg{diameterRe};
        parseAny(fs, diameter, reg);
        dynamic_cast<Circle*>(result)->radius=fieldToInt(diameter[0])/2;
    }

    result->id=shape_id;

    std::vector<std::string> params{4};
    std::vector<std::regex> reg{bmRe, ltRe, rtRe, tpRe};
    parseAny(fs, params, reg);
    result->bm=fieldToInt(params[0]);
    result->lt=fieldToInt(params[1]);
    result->rt=fieldToInt(params[2]);
    result->tp=fieldToInt(params[3]);

    if(shape_type==ShapeType::Circle)
    {
        std::vector<std::string> coords{2};
        std::vector<std::regex> reg{xRe, yRe};
        parseAny(fs, coords, reg);
        dynamic_cast<Circle*>(result)->coords.first=fieldToInt(coords[0]);
        dynamic_cast<Circle*>(result)->coords.second=fieldToInt(coords[1]);
        return result;
    }

    std::vector<std::string> vCount{1};
    reg.clear();
    reg.push_back(vertexCountRe);
    parseAny(fs, vCount, reg);
    int vertecies=fieldToInt(vCount[0]);
    dynamic_cast<Polygon*>(result)->vert.reserve(vertecies);
    for(int i=0; i<vertecies; i++)
    {
        std::vector<std::string> coords{3};
        std::vector<std::regex> reg{vertexIdRe, xRe, yRe};
        parseAny(fs, coords, reg);
        Vertex vert;
        vert.id=fieldToInt(coords[0], "-");
        vert.coords.first=fieldToInt(coords[1]);
        vert.coords.second=fieldToInt(coords[2]);
        dynamic_cast<Polygon*>(result)->vert.push_back(vert);
    }

    return result;
}


Layer parseLayer(std::ifstream& fs, int layer_id)
{
    Layer result;
    result.id=layer_id;
    std::string line;
    while(!std::regex_search(line, std::regex("Layer-"+std::to_string(layer_id))))
    {
        std::getline(fs, line);
    }

    std::vector<std::string> layerNum{1};
    std::vector<std::regex> reg{layerNumRe};
    parseAny(fs, layerNum, reg);
    int layers=fieldToInt(layerNum[0]);
    for(int i=0; i<layers; i++)
    {
        std::vector<std::string> id{1};
        std::vector<std::regex> reg{IdRe};
        parseAny(fs, id, reg);
        result.subLayers.push_back(fieldToInt(id[0]));
    }
    return result;
}

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
        result.emplace_back(ray.level, circle.y + circle.r);
        result.emplace_back(circle.x, circle.y + circle.r);
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