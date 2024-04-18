#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include "MLayer.h"
#include "Layer.h"
#include "utils.h"
#include <algorithm>

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

    std::vector<MLayer> subLayers=parseAllNonEmptyMLayers(fs); //only with at least one shape on them


    std::cout<<"parced non-empty mlayers:"<<std::endl;
    std::for_each(subLayers.begin(), subLayers.end(), [](MLayer& ml) {ml.print();});
    fs.close();

    MLayer polyLayer;
    for(auto& mlayer: subLayers)
    {
        fs.open("test_Polyfin.txt");
        bool isFound=false;
        for(auto& shapeId: mlayer.shapeMap)
        {
            Shape* sh=parseShape(fs, shapeId, ShapeType::Polygon);
            if(sh!=nullptr)
            {
                if(sh->type==ShapeType::Polygon)
                {
                    polyLayer=mlayer;
                    isFound=true;
                    break;
                }
            }
        }
        fs.close();
        if(isFound) break;
    }
    std::cout<<std::endl<<"the poly layer:"<<std::endl;
    polyLayer.print();

    fs.open("test_Polyfin.txt");
    std::cout<<"his parent:"<<std::endl;
    Layer parent=parseLayer(fs, polyLayer.parentId);
    parent.print();

    return 0;
}
