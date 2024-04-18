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
    fs.open("test_Polyfin.txt");
    Shape* sh=parseShape(fs, 99, ShapeType::Region);
    sh->print();
    delete sh;

    return 0;
}
