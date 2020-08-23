#pragma once

#include "vmfelements.h"
#include "pt/event.hpp"

#include "sstream"

namespace VMF{

class VMFImporter
{
public:
    struct JobResult
    {
        JobResult(const std::string filename):
            path(filename),
            scene(nullptr)
        {
        }

        bool isSuccessful() const{
            return scene != nullptr;
        }

        const std::string   path;
        std::string         result_msg;
        VMF::Scene*         scene;
    };

    VMFImporter();

    JobResult   LoadFile(const std::string filename);

private:
    bool        validatePath(const std::string& path) const;
    JobResult   ImportJob(const std::string path);
    void        ParseVMFFile(std::fstream& fs, JobResult& result);

    std::string getTrimmedLine(std::fstream& fs);

    int64_t     ParseInteger(const std::string& str);
    void        ParsePlane(const std::string& str, std::vector<PT::math::float3>& out_vec);
    PT::math::float3 ParseVec3(const std::string& str);


    void        ParseUnknownElement(std::fstream& fs, JobResult& result);

    VMF::World* ParseWorld(std::fstream& fs, JobResult& result);
    VMF::Solid* ParseSolid(std::fstream& fs, JobResult& result);
    VMF::Side*  ParseSide(std::fstream& fs, JobResult& result);


    std::vector<JobResult> results;

    Scene*  createScene();
    World* createWorld();
    Solid* createSolid();
    Side* createSide();

    std::vector<Scene*> scenes;
    std::vector<World*> worlds;
    std::vector<Solid*> solids;
    std::vector<Side*>  sides;

};

}
