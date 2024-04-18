#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include "MLayer.h"
#include "Layer.h"
#include "utils.h"
#include <algorithm>
#include "Xyr.h"

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

    const std::regex MLayerRe{"\"MLayer-[0-9]+\""};
    const std::regex IdRe{"\"Id\":[\\s\t]*[0-9]+"};
    const std::regex ParentIdRe{"\"Parent id\":[\\s\t]*[0-9]+"};
    const std::regex ShapeMapRe{"\"Shape maps\""};

    //parcing all non empty mlayers
    std::unordered_map<int, MLayer> subLayers=parseAllNonEmptyMLayers(fs); //only with at least one shape on them


    //std::cout<<"parced non-empty mlayers:"<<std::endl;
    //std::for_each(subLayers.begin(), subLayers.end(), [](MLayer& ml) {ml.print();});
    fs.close();

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
    //std::cout<<std::endl<<"the poly layer:"<<std::endl;
    //polyLayer.print();

    //looking for its parent layer
    fs.open("test_Polyfin.txt");
    //std::cout<<"his parent:"<<std::endl;
    Layer parent=parseLayer(fs, polyLayer.parentId);
    //parent.print();

    std::vector<Xyr> toWrite;

    for(int i=0; i<polygon->vert.size(); i++)
    {
        Xyr xyr;
        xyr.x=polygon->vert[i].coords.first/1000;
        xyr.y=polygon->vert[i].coords.second/1000;
        toWrite.push_back(xyr);
        xyr.print();
    }
    fs.close();
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
                        xyr.r=-dynamic_cast<Circle*>(sh)->radius/1000;
                        toWrite.push_back(xyr);
                        xyr.print();
                    }

                }
                fs.close();
            }
        }
    }
    return 0;
}
