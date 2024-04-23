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
4. Going through all sub-layers looking for circles
5. Saving data about their coordinates
6. Than based on vertecies of polygon and circle coordinates forming path
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

    std::vector<Xyr> circles;
    for(auto& id: parent.subLayers)
    {
        if(id!=polyLayer.id && subLayers.contains(id))
        {
            MLayer ml=subLayers[id];
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
    }


    std::vector<Xyr> result;
    //Now algo itself
    //traversing from left to right
    //then from up to down
    //then from right to left
    //and finally from down to up
    //forming sequence of dots on the way

    result = traverseFromTo(rayType::Horizontal, true, polyVert, circles);

    auto up_to_down = traverseFromTo(rayType::Vertical, false, polyVert, circles);
    std::copy(up_to_down.begin(), up_to_down.end(), std::back_inserter<std::vector<Xyr>>(result));
    
    auto right_to_left = traverseFromTo(rayType::Horizontal, false, polyVert, circles);
    std::copy(right_to_left.begin(), right_to_left.end(), std::back_inserter<std::vector<Xyr>>(result));
    
    auto down_to_up = traverseFromTo(rayType::Vertical, true, polyVert, circles);
    std::copy(down_to_up.begin(), down_to_up.end(), std::back_inserter<std::vector<Xyr>>(result));

    
    std::cout << "result sequence:" << std::endl;
    for (auto& res : result)
    {
        res.print();
    }

    std::ofstream to_drop{ "../../../Layoutfinal.hkp" };
    drop_results(result, to_drop);
    return 0;
}
