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

    std::vector<MLayer> subLayers; //only with at least one shape on them

    std::string line;
    while (std::getline(fs, line)) {
        std::smatch matchMlayer;
        std::regex_search(line, matchMlayer, MLayerRe);
        if(matchMlayer.str()!="")
        {
            MLayer mlayer;
            //std::cout<<matchMlayer.str()<<std::endl;
            std::string id;
            std::string parent_id;
            while(id=="" || parent_id=="")
            {
                std::smatch id_matcher;
                std::smatch parent_id_matcher;

                std::getline(fs, line);

                std::regex_search(line, id_matcher, IdRe);
                std::regex_search(line, parent_id_matcher, ParentIdRe);

                if(id=="") id=id_matcher.str();
                if(parent_id=="") parent_id=parent_id_matcher.str();
            }
            //std::cout<<fieldToInt(id)<<std::endl;
            //std::cout<<fieldToInt(parent_id)<<std::endl;
            mlayer.id=fieldToInt(id);
            mlayer.parentId=fieldToInt(parent_id);
            while(!line.contains("Text map"))
            {
                std::smatch shape_id_matcher;
                std::getline(fs, line);
                std::regex_search(line, shape_id_matcher, IdRe);
                if(shape_id_matcher.str()!="") mlayer.shapeMap.push_back(fieldToInt(shape_id_matcher.str()));
            }
            if(mlayer.shapeMap.size()!=0) subLayers.push_back(mlayer);
        }
    }

    std::cout<<"parced non-empty mlayers:"<<std::endl;
    std::for_each(subLayers.begin(), subLayers.end(), [](MLayer& ml) {ml.print();});


    return 0;
}
