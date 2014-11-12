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

    struct waypoint {
		irr::core::vector3df position;
		waypoint* connected[10];
    };

    std::vector<waypoint*> waypoints;
};

RMesh* loadRMesh(std::string path,irr::io::IFileSystem* fs,irr::video::IVideoDriver* driver,irr::video::E_MATERIAL_TYPE RoomShader);

#endif // RMESH_H_INCLUDED
