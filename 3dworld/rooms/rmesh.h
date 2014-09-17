#ifndef RMESH_H_INCLUDED
#define RMESH_H_INCLUDED

#include "irrlicht.h"
#include "IrrlichtDevice.h"
#include "IFileSystem.h"
#include "SMesh.h"
#include "IVideoDriver.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <iostream>
#include <vector>
#include <string>

struct pointlight {
    irr::core::vector3df position;
    float radius;
    irr::video::SColorf color;
};

struct RMesh {
    std::string path;
    irr::scene::IMesh* mesh;
    btBvhTriangleMeshShape* shape;
    std::vector<pointlight> pointlights;
    std::vector<irr::core::vector3df> waypoints;
};

RMesh* loadRMesh(const std::string &path,irr::io::IFileSystem* fs,irr::video::IVideoDriver* driver);

#endif // RMESH_H_INCLUDED
