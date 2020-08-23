#pragma once

#include <cstddef>

#include <vector>

#include "math/math.h"

namespace VMF{

struct EditorProperty
{
    PT::math::float3    color;
    bool                visgroupshown;      //???
    bool                visgroupautoshown;  //???
    PT::math::float2*   logicalpos;

};

struct HasEditorProperties
{
    EditorProperty*  editorproperties;
};

struct Element
{
//struct VMFElement: HasEditorProperties{
    uint64_t            id;

};



struct Side: public Element
{
    PT::math::float3    plane_points[3];
    std::string         material;
    PT::math::float4    uaxis;      //[1,  0, 0, 'ushift'] //used for texture-shifts
    PT::math::float4    vaxis;      //[0, -1, 0, 'vshift'] //used for texture-shifts
    float               uscale;
    float               vscale;
    float               rotation;
    float               lightmapscale;
    //???               smoothing_groups
};



struct Solid: public Element, public HasEditorProperties
{
    std::vector<Side*> sides;


};



struct Camera
{
    PT::math::float3 position;
    PT::math::float3 look;
};

struct Cameras
{
    int                     activecamera;
    std::vector<Camera>  cameras;
};

struct Cordon
{
    PT::math::float3 mins;
    PT::math::float3 maxs;
    //???           active;
};


enum class EntityType
{
    UNKNOWN = 0,
    FUNC_DETAIL,

};

struct Entity: public Element, public HasEditorProperties
{
    EntityType              classname;
    PT::math::float3        origin;

};

struct World: public Element, public HasEditorProperties
{
    uint64_t                mapversion;
    std::string             classname;  //"worldspawn"
    std::string             skyname;
    //???                   maxpropscreenwidth; // -1
    std::string             detailvbsp;
    std::string             detailmaterial;
    std::vector<Solid*>     solids;

};

struct Scene{
    //versioninfo
    //visgroups
    //viewsettings

    World*               world;
    std::vector<Entity>  entities;
    Cameras*             cameras;
    Cordon*              cordon;

};


} //end of VMF namespace

