#include <Parcing.h>
#include <regex>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
#include <limits>
#include <string>
#include <chrono>
#include <ctime>

int fieldToInt(const std::string& field, const std::string& delimeter)
{
    return std::stoi(field.substr(field.find(delimeter) + 1, field.length()));
}

bool isEmpty(std::string& str) { return str.empty(); }

void parseAny(std::ifstream& fs, std::vector<std::string>& results, const std::vector<std::regex>& reg)
{
    std::string line;
    while (std::any_of(results.begin(), results.end(), isEmpty))
    {
        std::vector<std::smatch> matchers{ results.size() };
        std::getline(fs, line);

        for (int i = 0; i < matchers.size(); i++)
        {
            std::regex_search(line, matchers[i], reg[i]);
            if (results[i].empty()) results[i] = matchers[i].str();
        }
    }
}

MLayer parseMLayer(std::ifstream& fs, int mlayer_id)
{


    std::string line;
    while (std::getline(fs, line)) 
    {
        std::smatch matchMlayer;
        std::regex_search(line, matchMlayer, MLayerRe);
        if (!matchMlayer.str().empty())
        {
            MLayer mlayer;
            std::vector<std::string> ids{ 2 };
            std::vector<std::regex> reg{ IdRe, ParentIdRe };
            parseAny(fs, ids, reg);

            if (fieldToInt(ids[0]) != mlayer_id) continue;
            mlayer.id = fieldToInt(ids[0]);
            mlayer.parentId = fieldToInt(ids[1]);
            
            while (!line.contains("Text map"))
            {
                std::smatch shape_id_matcher;
                std::getline(fs, line);
                std::regex_search(line, shape_id_matcher, IdRe);
                if (!shape_id_matcher.str().empty()) mlayer.shapeMap.push_back(fieldToInt(shape_id_matcher.str()));
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
    while (std::getline(fs, line)) 
    {
        std::smatch matchMlayer;
        std::regex_search(line, matchMlayer, MLayerRe);
        if (!matchMlayer.str().empty())
        {
            MLayer mlayer;
            std::vector<std::string> ids{ 2 };
            std::vector<std::regex> reg{ IdRe, ParentIdRe };
            parseAny(fs, ids, reg);
            mlayer.id = fieldToInt(ids[0]);
            mlayer.parentId = fieldToInt(ids[1]);
            
            while (!line.contains("Text map"))
            {
                std::smatch shape_id_matcher;
                std::getline(fs, line);
                std::regex_search(line, shape_id_matcher, IdRe);
                if (!shape_id_matcher.str().empty()) mlayer.shapeMap.push_back(fieldToInt(shape_id_matcher.str()));
            }
            if (mlayer.shapeMap.size() != 0) result.insert({ mlayer.id, mlayer });
        }
    }
    return result;
}


Shape* parseShape(std::ifstream& fs, int shape_id, ShapeType shape_type)
{
    Shape* result;
    switch (shape_type)
    {
    case ShapeType::Circle:
        result = new Circle();
        break;
    case ShapeType::Polygon:
        result = new Polygon();
        break;
    case ShapeType::Region:
        result = new Polygon();
        result->type = ShapeType::Region;
        break;
    }

    std::regex shapeRe{ "\"" + shapeTypeToString(shape_type) + "-" + std::to_string(shape_id) + "\"" };
    std::string line;
    std::smatch shape_matcher;
    while (std::getline(fs, line)) {
        std::regex_search(line, shape_matcher, shapeRe);
        if (!shape_matcher.str().empty()) break;
    }
    if (shape_matcher.str().empty())
    {
        delete result;
        result = nullptr;
        return result;
    }

    if (shape_type == ShapeType::Circle)
    {
        std::vector<std::string> diameter{ 1 };
        std::vector<std::regex> reg{ diameterRe };
        parseAny(fs, diameter, reg);
        dynamic_cast<Circle*>(result)->radius = fieldToInt(diameter[0]) / 2;
    }

    result->id = shape_id;

    std::vector<std::string> params{ 4 };
    std::vector<std::regex> reg{ bmRe, ltRe, rtRe, tpRe };
    parseAny(fs, params, reg);
    result->bm = fieldToInt(params[0]);
    result->lt = fieldToInt(params[1]);
    result->rt = fieldToInt(params[2]);
    result->tp = fieldToInt(params[3]);

    if (shape_type == ShapeType::Circle)
    {
        std::vector<std::string> coords{ 2 };
        std::vector<std::regex> reg{ xRe, yRe };
        parseAny(fs, coords, reg);
        dynamic_cast<Circle*>(result)->coords.first = fieldToInt(coords[0]);
        dynamic_cast<Circle*>(result)->coords.second = fieldToInt(coords[1]);
        return result;
    }

    std::vector<std::string> vCount{ 1 };
    reg.clear();
    reg.push_back(vertexCountRe);
    parseAny(fs, vCount, reg);

    int vertecies = fieldToInt(vCount[0]);
    dynamic_cast<Polygon*>(result)->vert.reserve(vertecies);
    for (int i = 0; i < vertecies; i++)
    {
        std::vector<std::string> coords{ 3 };
        std::vector<std::regex> reg{ vertexIdRe, xRe, yRe };
        parseAny(fs, coords, reg);
        Vertex vert;
        vert.id = fieldToInt(coords[0], "-");
        vert.coords.first = fieldToInt(coords[1]);
        vert.coords.second = fieldToInt(coords[2]);
        dynamic_cast<Polygon*>(result)->vert.push_back(vert);
    }

    return result;
}


Layer parseLayer(std::ifstream& fs, int layer_id)
{
    Layer result;
    result.id = layer_id;
    std::string line;
    while (!std::regex_search(line, std::regex("Layer-" + std::to_string(layer_id))))
    {
        std::getline(fs, line);
    }

    std::vector<std::string> layerNum{ 1 };
    std::vector<std::regex> reg{ layerNumRe };
    parseAny(fs, layerNum, reg);
    int layers = fieldToInt(layerNum[0]);
    for (int i = 0; i < layers; i++)
    {
        std::vector<std::string> id{ 1 };
        std::vector<std::regex> reg{ IdRe };
        parseAny(fs, id, reg);
        result.subLayers.push_back(fieldToInt(id[0]));
    }
    return result;
}

void drop_results(const std::vector<Xyr>& result, std::ofstream& fs)
{
    std::ifstream template_prefix{"layout_templates/layout_prefix.hkp"};

    std::string prefix;
    std::string line;

    while (!template_prefix.eof())
    {
        std::getline(template_prefix, line);
        prefix += line+"\n";
    }

    auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto time=std::localtime(&time_t);
    

    std::string short_date = std::format("{}.0{}", std::to_string(time->tm_mday), std::to_string(time->tm_mon + 1));
    prefix=std::regex_replace(prefix, std::regex("\\{date_short\\}"), short_date);
    prefix = std::regex_replace(prefix, std::regex("\\{nick_name\\}"), "kargamant");
    
    std::ostringstream oss;
    oss << std::put_time(time, "%A, %B %d, %Y %H:%M");
    prefix = std::regex_replace(prefix, std::regex("\\{full_date\\}"), oss.str());
    prefix = std::regex_replace(prefix, std::regex("\\{ts\\}"), std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()));
    
    fs << prefix;

    for (auto res : result)
    {
        fs << std::string(11, ' ');
        res.print(fs);
    }

    std::ifstream template_postfix{"layout_templates/layout_postfix.hkp" };
    std::string postfix;

    while (!template_postfix.eof())
    {
        std::getline(template_postfix, line);
        postfix += line + "\n";
    }
    postfix=std::regex_replace(postfix, std::regex("\\{author\\}"), "firexholms@gmail.com");
    fs << postfix;
    fs.close();
}