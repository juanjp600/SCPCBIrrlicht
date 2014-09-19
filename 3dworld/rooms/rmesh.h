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

struct RMesh {
    std::string path;
    irr::scene::IMesh* mesh;
    btBvhTriangleMeshShape* shape;
    std::vector<irr::video::SLight> pointlights;
    std::vector<irr::core::vector3df> waypoints;
};

RMesh* loadRMesh(std::string path,irr::io::IFileSystem* fs,irr::video::IVideoDriver* driver);

#endif // RMESH_H_INCLUDED
