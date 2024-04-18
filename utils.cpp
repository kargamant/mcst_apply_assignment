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


Shape* parseShape(std::ifstream& fs, int shape_id, ShapeType shape_type)
{
    Shape* result;
    switch(shape_type)
    {
    case ShapeType::Circle:
        result=new Circle();
    case ShapeType::Polygon:
        result=new Polygon();
    case ShapeType::Region:
        result=new Polygon();
        result->type=ShapeType::Region;
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
        return nullptr;
    }

    if(shape_type==ShapeType::Circle)
    {
        std::string diameter;
        while(diameter=="")
        {
            std::smatch diameter_matcher;

            std::getline(fs, line);

            std::regex_search(line, diameter_matcher, diameterRe);
            diameter=diameter_matcher.str();
        }
        static_cast<Circle*>(result)->radius=fieldToInt(diameter)/2;
    }

    result->id=shape_id;
    std::string bm;
    std::string lt;
    std::string rt;
    std::string tp;
    while(bm=="" || lt=="" || rt=="" || tp=="")
    {
        std::smatch bm_matcher;
        std::smatch lt_matcher;
        std::smatch rt_matcher;
        std::smatch tp_matcher;
        std::getline(fs, line);

        std::regex_search(line, bm_matcher, bmRe);
        std::regex_search(line, lt_matcher, ltRe);
        std::regex_search(line, rt_matcher, rtRe);
        std::regex_search(line, tp_matcher, tpRe);
        if(bm=="") bm=bm_matcher.str();
        if(lt=="") lt=lt_matcher.str();
        if(rt=="") rt=rt_matcher.str();
        if(tp=="") tp=tp_matcher.str();
    }
    result->bm=fieldToInt(bm);
    result->lt=fieldToInt(lt);
    result->rt=fieldToInt(rt);
    result->tp=fieldToInt(tp);
    return result;
}
