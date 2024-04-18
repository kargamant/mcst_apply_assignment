#include "utils.h"
#include <regex>

int fieldToInt(const std::string& field)
{
    return std::stoi(field.substr(field.find(":")+1, field.length()));
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
            if(fieldToInt(id)!=mlayer_id) continue;
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
            return mlayer;
        }
    }
    return MLayer();
}

std::vector<MLayer> parseAllNonEmptyMLayers(std::ifstream& fs)
{
    std::vector<MLayer> result;
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
            if(mlayer.shapeMap.size()!=0) result.push_back(mlayer);
        }
    }
    return result;
}
