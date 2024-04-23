#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include "MLayer.h"
#include "Layer.h"
#include "traversing.h"
#include <algorithm>
#include "Xyr.h"
#include <set>
#include "Ray.h"
#include <stack>
#include "Parcing.h"

/*
 Algo described:
1. Looking for MLayer with polygon(saving id and parent id)
2. Looking for his parent Layer-<parent_id>
3. Saving all sub-layers ids
4. Going through all sub-layers looking for shapes
5. Saving data on the way
*/

int main()
{
    std::ifstream fs{"test_Polyfin.txt"};



    //parcing all non empty mlayers
    std::unordered_map<int, MLayer> subLayers=parseAllNonEmptyMLayers(fs); //only with at least one shape on them


    //looking for mlayer with polygon
    MLayer polyLayer;
    Polygon* polygon;
    for(auto& mlayer: subLayers)
    {
        fs.open("test_Polyfin.txt");
        bool isFound=false;
        for(auto& shapeId: mlayer.second.shapeMap)
        {
            Shape* sh=parseShape(fs, shapeId, ShapeType::Polygon);
            if(sh!=nullptr)
            {
                if(sh->type==ShapeType::Polygon)
                {
                    polygon=dynamic_cast<Polygon*>(sh);
                    polyLayer=mlayer.second;
                    isFound=true;
                    break;
                }
                //delete sh;
            }
        }
        fs.close();
        if(isFound) break;
    }

    
    //forming vector of polygon vertecies
    std::vector<Xyr> polyVert;
    for (int i = 0; i < polygon->vert.size(); i++)
    {
        Xyr xyr;
        xyr.x = polygon->vert[i].coords.first / 1000;
        xyr.y = polygon->vert[i].coords.second / 1000;
        polyVert.push_back(xyr);
        xyr.print();
    }

    
    //looking for its parent layer
    fs.open("test_Polyfin.txt");
    Layer parent=parseLayer(fs, polyLayer.parentId);
    fs.close();

    
    //looking for ventholes in parent sub layers

    //fs.open("test_Polyfin.txt");
    std::vector<Xyr> circles;
    for(auto& id: parent.subLayers)
    {
        //fs.open("test_Polyfin.txt");
        if(id!=polyLayer.id && subLayers.contains(id))
        {
            MLayer ml=subLayers[id];
            //int last_id = 0;
            for(auto& shapeId: ml.shapeMap)
            {
                fs.open("test_Polyfin.txt");
                Shape* sh=parseShape(fs, shapeId, ShapeType::Circle);
                if(sh!=nullptr)
                {
                    if(sh->type==ShapeType::Circle)
                    {
                        Xyr xyr;
                        xyr.x=dynamic_cast<Circle*>(sh)->coords.first/1000;
                        xyr.y=dynamic_cast<Circle*>(sh)->coords.second/1000;
                        xyr.r=dynamic_cast<Circle*>(sh)->radius/1000;
                        circles.push_back(xyr);
                        xyr.print();
                    }

                }
                fs.close();
            }
        }
        //fs.close();
    }


    //circle rays
    std::set<Ray> circleHorizontalRays;
    std::set<Ray> circleVerticalRays;
    for(auto& circle: circles)
    {
        if(!circleHorizontalRays.contains(Ray(rayType::Horizontal, circle.y, circle.x))) circleHorizontalRays.insert(Ray(rayType::Horizontal, circle.y, circle.x));
        if(!circleVerticalRays.contains(Ray(rayType::Vertical, circle.x, circle.y))) circleVerticalRays.insert(Ray(rayType::Vertical, circle.x, circle.y));
    }

    for (auto& ray : circleHorizontalRays)
    {
        ray.print();
    }
    for (auto& ray : circleVerticalRays)
    {
        ray.print();
    }


    //polygon rays
    std::set<Ray> polyHorizontalRays;
    std::set<Ray> polyVerticalRays;
    for (auto& vert : polyVert)
    {
        if (!polyHorizontalRays.contains(Ray(rayType::Horizontal, vert.y, vert.x))) polyHorizontalRays.insert(Ray(rayType::Horizontal, vert.y, vert.x));
        if (!polyVerticalRays.contains(Ray(rayType::Vertical, vert.x, vert.y))) polyVerticalRays.insert(Ray(rayType::Vertical, vert.x, vert.y));
    }

    for (auto& ray : polyHorizontalRays)
    {
        ray.print();
    }
    for (auto& ray : polyVerticalRays)
    {
        ray.print();
    }


    std::vector<Xyr> result;

    
    //Now algo itself
    //traversing from left to right


    result = traverseFromTo(rayType::Horizontal, true, polyVert, circles);
    auto up_to_down = traverseFromTo(rayType::Vertical, false, polyVert, circles);
    std::copy(up_to_down.begin(), up_to_down.end(), std::back_inserter<std::vector<Xyr>>(result));
    auto right_to_left = traverseFromTo(rayType::Horizontal, false, polyVert, circles);
    std::copy(right_to_left.begin(), right_to_left.end(), std::back_inserter<std::vector<Xyr>>(result));
    auto down_to_up = traverseFromTo(rayType::Vertical, true, polyVert, circles);
    std::copy(down_to_up.begin(), down_to_up.end(), std::back_inserter<std::vector<Xyr>>(result));

    
    std::cout << "left->right sequence:" << std::endl;
    for (auto& res : result)
    {
        res.print();
    }

    std::ofstream to_drop{ "../../../Layoutfinal.hkp" };
    drop_results(result, to_drop);

       /*for (int v = 0; v<polyVert.size(); v++)
    {
        if(polyVert[v].x==po)
        for (int i = 0; i < circles.size(); i++)
        {
            if (circles[i].x > vert.x)
            {
                int checkY = circles[i].y;
                bool skip = false;
                for (auto& ray: polyHorizontalRays)
                {
                    if (ray.level != vert.y && ray.level > checkY)
                    {
                        skip = true;
                        break;
                    }
                }
                if (skip) continue;
                else
                {

                }
            }
        }
    }*/
    

    /*std::set<int> horizontalRays;
    std::set<int> verticalRays;
    for(auto& circle: circles)
    {
        if(!horizontalRays.contains(circle.y)) horizontalRays.insert(circle.y);
        if(!verticalRays.contains(circle.x)) verticalRays.insert(circle.x);
    }


    std::cout<<"horizontal rays:"<<std::endl;
    for(auto& ray: horizontalRays)
    {
        std::cout<<ray<<std::endl;
    }
    std::cout<<"vertical rays:"<<std::endl;
    for(auto& ray: verticalRays)
    {
        std::cout<<ray<<std::endl;
    }
    std::vector<int> horizontalBorders;
    std::vector<int> verticalBorders;
    std::vector<Xyr> path;
    for(int i=0; i<polyVert.size()-1; i++)
    {
        if(polyVert[i].y==polyVert[i+1].y)
        {
            horizontalBorders.push_back(polyVert[i].y);
        }
        else if(polyVert[i].x==polyVert[i+1].x)
        {
            verticalBorders.push_back(polyVert[i].x);
        }
    }

    std::cout<<"horizontal_borders:"<<std::endl;
    for(auto& b: horizontalBorders) std::cout<<b<<std::endl;
    std::cout<<"vertical_borders:"<<std::endl;
    for(auto& b: verticalBorders) std::cout<<b<<std::endl;
*/
    //or consider projections and finding closest border
    //So
    /*
     1. Going through polygon vertecies.
     2. Adding current vertex to result vector
     3. If the next vertex have the same Y coordinate, then current-next is horizontal border.
          Looking through circles that have X between current and next vertex.
          Forming sequence of dots for each circle(the farest from border are processed first) and adding this sequence to the result vector of xyr.
          Marking circles as connected, so we dont touch them anymore
     4. If the next vertex have the same X coordinate, then current-next is vertical border
           Looking through circles that have Y between current and next vertex
           Forming sequence of dots for each circle(the farest from border are processed first) and adding this sequence to the result vector xyr
           Marking circles as connected, so we dont touch them anymore
     5. If the next vertex does not have same coordinate with current,
        this means that current-next border is neither horizontal nor vertical and should be ignored.
*/
    //It will be cool to have two vectors of circles, one sorted by X and other by Y.



    return 0;
}
