#include "vmfimporter.h"

#include <fstream>

#include "pt/std_extension.hpp"

using namespace VMF;

VMFImporter::
VMFImporter()
{

}

bool VMFImporter::
validatePath(const std::string &path) const
{
    if(path.length() <= 0){ return false; }

    return true; //ptoth todo
}

VMFImporter::JobResult VMFImporter::
LoadFile(std::string filename)
{
    return ImportJob(filename);
}


VMFImporter::JobResult VMFImporter::
ImportJob(std::string path)
{
    JobResult result(path);
    std::stringstream ss;

    //validate path
    if(!validatePath(path)){
        ss << "invalid path: " << path;
        throw std::invalid_argument(ss.str());
    }


    //try opening file
    std::fstream fs(path);
    if(!fs.is_open()){
        ss << "could not open file: " << path;
        throw std::invalid_argument(ss.str());
    }

    //read data
    ParseVMFFile(fs, result);

    //return results
    return result;
}

std::string VMFImporter::
getTrimmedLine(std::fstream &fs)
{
    std::string line;
    std::getline(fs, line);
    return PT::trimWhitespaces(line);
}

int64_t VMFImporter::
ParseInteger(const std::string &str)
{
    std::stringstream ss;
    std::string outstr;
    //remove first and last characters if they are quotation marks
    if(str.length()<=0){
        ss << "integer parse failed: " << str;
        throw std::runtime_error(ss.str());
    }

    if(str[0] == '"'){
        outstr = str.substr(1, str.length()-2);
    }else{
        outstr = str;
    }

    int64_t temp = atoll(outstr.c_str());
    return temp;
}



void VMFImporter::
ParsePlane(const std::string &str, std::vector<PT::math::float3> &out_vec)
{
    std::stringstream ss;
    std::string innerstr;
    std::string split[2];
    std::string vectors[3];



    if(str.length()<=0){
        ss << "plane parse failed: " << str;
        throw std::runtime_error(ss.str());
    }
    innerstr = str.substr(2, str.length() -4);


    PT::splitString(split, innerstr, ") (");
    vectors[0] = split[0];
    PT::splitString(&vectors[1], split[1], ") (");

    out_vec[0] = ParseVec3(vectors[0]);
    out_vec[1] = ParseVec3(vectors[1]);
    out_vec[2] = ParseVec3(vectors[2]);
}

PT::math::float3 VMFImporter::
ParseVec3(const std::string &str)
{
    PT::math::float3 v3;
    std::string split[2];

    PT::splitString(split, str, " ");
    v3.x = ParseInteger(split[0]);

    PT::splitString(split, split[1], " ");
    v3.y = ParseInteger(split[0]);
    v3.z = ParseInteger(split[1]);
    return v3;
}


void VMFImporter::
ParseVMFFile(std::fstream &fs, VMFImporter::JobResult &result)
{
    std::string line;
    std::stringstream ss;

    VMF::Scene* scene = createScene();

    while(!fs.eof()){
        line = getTrimmedLine(fs);
        if(line == "world"){
            if(line == "") continue;
            else if(scene->world != nullptr){
                ss << "multiple 'world' elements present";
                throw std::logic_error(ss.str());
            }

            scene->world = ParseWorld(fs, result);
        }else{
            ParseUnknownElement(fs, result);
        }
    }
    result.scene = scene;
}


//ptoth: watch out! may get called, when it's not immediately followed by a '{'
//      it'll mess up the bracecount in those cases
void VMFImporter::
ParseUnknownElement(std::fstream &fs, VMFImporter::JobResult &result)
{
    //-----

    //-----
    std::stringstream ss;
    std::string line;
    int bracecounter = 0;
    while(!fs.eof()){
        line = getTrimmedLine(fs);
        if(line == "") continue;
        else if(line == "{") ++bracecounter;
        else if(line == "}"){
            --bracecounter;
            if(bracecounter == 0){
                return;
            }
        }
        //-----

        //-----
    }
    if(0 < bracecounter){
        ss << "file is corrupt";
        throw std::runtime_error(ss.str());
    }
}



World *VMFImporter::
ParseWorld(std::fstream &fs, VMFImporter::JobResult &result)
{
    //-----
    VMF::World* world = createWorld();

    //-----
    std::stringstream ss;
    std::string line;
    int bracecounter = 0;
    while(!fs.eof()){
        line = getTrimmedLine(fs);
        if(line == "") continue;
        else if(line == "{") ++bracecounter;
        else if(line == "}"){
            --bracecounter;
            if(bracecounter == 0){
                return world;
            }
        }
        //-----

        if(line == "solid"){
            VMF::Solid* solid = ParseSolid(fs, result);
            world->solids.push_back(solid);
        }
        //-----
    }
    ss << "file is corrupt";
    throw std::runtime_error(ss.str());
}

Solid *VMFImporter::
ParseSolid(std::fstream &fs, VMFImporter::JobResult &result)
{
    //-----
    Solid* solid = createSolid();


    //-----
    std::stringstream ss;
    std::string line;
    int bracecounter = 0;
    while(!fs.eof()){
        line = getTrimmedLine(fs);
        if(line == "") continue;
        else if(line == "{") ++bracecounter;
        else if(line == "}"){
            --bracecounter;
            if(bracecounter == 0){
                return solid;
            }
        }
        //-----
        else{
            std::string split[2];
            bool splitres = PT::splitString(split, line, " ");
            if(!splitres){ split[0] = line; }
            if(split[0] == "\"id\""){
                int64_t i64 = ParseInteger(split[1]);
                if(i64 < 0) throw std::logic_error("id cannot be negative");
                solid->id = static_cast<uint64_t>(i64);
            }else if(split[0] == "side"){
                Side* side = ParseSide(fs, result);
                solid->sides.push_back(side);
            }else{
                ParseUnknownElement(fs, result);
            }
        }
        //-----
    }
    ss << "file is corrupt";
    throw std::runtime_error(ss.str());
}

Side *VMFImporter::
ParseSide(std::fstream &fs, VMFImporter::JobResult &result)
{
    //-----
    Side* side = createSide();

    //-----
    std::stringstream ss;
    std::string line;
    int bracecounter = 0;
    while(!fs.eof()){
        line = getTrimmedLine(fs);
        if(line == "") continue;
        else if(line == "{") ++bracecounter;
        else if(line == "}"){
            --bracecounter;
            if(bracecounter == 0){
                return side;
            }
        }
        //-----
        else{
            std::string split[2];
            bool splitres = PT::splitString(split, line, " ");
            if(!splitres){ split[0] = line; }
            if(split[0] == "\"id\""){
                int64_t i64 = ParseInteger(split[1]);
                if(i64 < 0) throw std::logic_error("id cannot be negative");
                side->id = static_cast<uint64_t>(i64);
            }else if(split[0] == "\"plane\""){
                std::vector<PT::math::float3> v3(3);
                ParsePlane(split[1], v3);
                side->plane_points[0] = v3[0];
                side->plane_points[1] = v3[1];
                side->plane_points[2] = v3[2];
            }else if(split[0] == "\"material\""){

            }else if(split[0] == "\"uaxis\""){

            }else if(split[0] == "\"vaxis\""){

            }else if(split[0] == "\"rotation\""){
                int64_t i64 = ParseInteger(split[1]);
                side->id = static_cast<uint64_t>(i64);

            }else if(split[0] == "\"lightmapscale\""){

            }else if(split[0] == "\"smoothing_groups\""){

            }else{
                ParseUnknownElement(fs, result);
            }
        }
        //-----
    }
    ss << "file is corrupt";
    throw std::runtime_error(ss.str());
}

Scene *VMFImporter::createScene()
{
    Scene* s = new Scene();
    scenes.push_back(s);
    return s;
}

World *VMFImporter::createWorld()
{
    World* w = new World();
    worlds.push_back(w);
    return w;
}

Solid *VMFImporter::createSolid()
{
    Solid* s = new Solid();
    solids.push_back(s);
    return s;
}

Side *VMFImporter::createSide()
{
    Side* s = new Side();
    sides.push_back(s);
    return s;
}


