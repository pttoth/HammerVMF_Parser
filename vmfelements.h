#pragma once

namespace VMF{

struct VMFPlane{

};

struct VMFSolid{

};

struct VMFCamera{};


enum class EntityType{
    UNKNOWN = 0,
    FUNC_DETAIL,

};

struct VMFEntity{
    EntityType type;
};

struct VMFWorld{
};

struct VMFScene{
    //versioninfo
    //visgroups
    //viewsettings

    //World
    //Entities ?
    //Cameras
    //cordon ???

};


class VMFElements
{

public:
    VMFElements();
};


} //end of VMF namespace

