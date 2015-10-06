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

#include "../ShaderCallbacks.h"

struct pointLight {
    irr::core::vector3df position;
    float radius;
    float intensity;
    irr::video::SColorf color;
    //irr::core::matrix4 viewMatrix[6];
};

struct RMesh {
    std::string path;
    irr::scene::IMesh* mesh;
    std::vector<irr::scene::IMesh*> transparentSurfaces; //these need to be separate so Irrlicht can sort them
    std::vector<irr::core::vector3df> transparentOffset;
    btBvhTriangleMeshShape* shape;
    std::vector<pointLight> pointlights;

    struct waypoint {
		irr::core::vector3df position;
		waypoint* connected[20];
    };

    std::vector<waypoint*> waypoints;
};

RMesh* loadRMesh(std::string path,irr::io::IFileSystem* fs,irr::scene::ISceneManager* smgr,irr::video::ITexture** roomTextures,irr::video::E_MATERIAL_TYPE* roomShaders);

#endif // RMESH_H_INCLUDED
