#pragma once

#include "vmfelements.h"

namespace VMF{

class VMFImportJobResult
{
public:
    VMFImportJobResult(int32_t id);
    int32_t         getID() const;
    std::string&    getResult() const;

private:
    int32_t         _id;
    std::string     _result;


};

class VMFImporter
{
public:
    VMFImporter();
    ~VMFImporter();

    //possible errors
    //  invalid path
    //  file does not exist
    //  file is not a VMF
    //  fatal parsing error (file is corrupt)

    VMFScene getJob(int32_t id);

    int32_t LoadFile(std::string filename);


};

}
