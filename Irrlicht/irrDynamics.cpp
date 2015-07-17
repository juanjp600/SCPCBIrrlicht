    /*
    irrDynamics - Light-weight Bullet Physics wrapper for the irrlicht graphics engine
    Copyright (C) 2014  Otto Naderer - otto.naderer@aec.at

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
    */
    //Modified to support trimeshes and filtering
    //Removed global variables

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "irrDynamics.h"

#include <iostream>
#include <vector>
#include <cmath>

btVector3 irrToBtVec(irr::core::vector3df inVec) {
    return btVector3(inVec.X,inVec.Y,inVec.Z);
}

irr::core::vector3df btToIrrVec(btVector3 inVec) {
    return irr::core::vector3df(inVec[0],inVec[1],inVec[2]);
}

using namespace irr;

irrDynamics::irrDynamics() : lastStep(0) {
	// Initialize bullet
    collisionConfiguration = new btDefaultCollisionConfiguration();
    broadPhase = new btAxisSweep3(btVector3(-10000, -10000, -10000), btVector3(10000, 10000, 10000));
    broadPhase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
    world->getDispatchInfo().m_allowedCcdPenetration=0.0001f;
}


btDiscreteDynamicsWorld* irrDynamics::getCollisionWorld() {
	return world;
}

void irrDynamics::simStep(f32 elapsedTime,f32 prec) {
    //if (lastStep == 0)
    //    lastStep = curTimeStamp;

    //if ((curTimeStamp - lastStep)<=0) return;
    world->stepSimulation(elapsedTime, 14.f*prec/60.f, 1.0/prec);

    updateObjects();
    //lastStep = curTimeStamp;
}

/*void irrDynamics::resetTimer(u32 curTimeStamp) {
	lastStep = curTimeStamp;
}*/

void irrDynamics::shutdown() {
    clearObjects();
    delete world;
    delete solver;
    delete dispatcher;
    delete broadPhase;
    delete collisionConfiguration;
}

btRigidBody* irrDynamics::addTriMesh_moving(irr::scene::IMeshSceneNode* node,f32 mass,unsigned int precisionLoss,short group,short mask) { //any concave stuff will become convex
	node->updateAbsolutePosition();
	core::vector3df irrPos = node->getPosition();
	btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btPos);

	core::vector3df* mOffset;
	mOffset = new core::vector3df(0.0f,0.0f,0.0f);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	irr::scene::IMesh* pMesh = node->getMesh();
	core::vector3df scaling = node->getScale();
	u32 i,k,numVertices;
	btConvexHullShape *mShape = new btConvexHullShape();

	f32 mx,Mx,my,My,mz,Mz;
	Mx = My = Mz = -INFINITY;
	mx = my = mz = INFINITY;

	u32 bufferCount = pMesh->getMeshBufferCount();
	for (i=0; i<bufferCount; i++) { //get mesh boundaries
		scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);
		if (mb->getVertexType()==video::EVT_STANDARD) {
			video::S3DVertex* mb_vertices=(video::S3DVertex*)mb->getVertices();
			//mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount()-precisionLoss+1;

			for (k=0;k<numVertices;k+=precisionLoss) {
				core::vector3df irrPos2 = mb_vertices[k].Pos;

				mx = std::min(mx,irrPos2.X * scaling.X);
				Mx = std::max(Mx,irrPos2.X * scaling.X);
				my = std::min(my,irrPos2.Y * scaling.Y);
				My = std::max(My,irrPos2.Y * scaling.Y);
				mz = std::min(mz,irrPos2.Z * scaling.Z);
				Mz = std::max(Mz,irrPos2.Z * scaling.Z);
			}
		} else if(mb->getVertexType()==video::EVT_2TCOORDS) {
			video::S3DVertex2TCoords* mb_vertices=(video::S3DVertex2TCoords*)mb->getVertices();
			numVertices = mb->getVertexCount()-precisionLoss+1;

			for (k=0;k<numVertices;k+=precisionLoss) {
				core::vector3df irrPos2 = mb_vertices[k].Pos;

				mx = std::min(mx,irrPos2.X * scaling.X);
				Mx = std::max(Mx,irrPos2.X * scaling.X);
				my = std::min(my,irrPos2.Y * scaling.Y);
				My = std::max(My,irrPos2.Y * scaling.Y);
				mz = std::min(mz,irrPos2.Z * scaling.Z);
				Mz = std::max(Mz,irrPos2.Z * scaling.Z);
			}
		} else if(mb->getVertexType()==video::EVT_TANGENTS) {
			video::S3DVertexTangents* mb_vertices=(video::S3DVertexTangents*)mb->getVertices();
			numVertices = mb->getVertexCount()-precisionLoss+1;

			for (k=0;k<numVertices;k+=precisionLoss) {
				core::vector3df irrPos2 = mb_vertices[k].Pos;

				mx = std::min(mx,irrPos2.X * scaling.X);
				Mx = std::max(Mx,irrPos2.X * scaling.X);
				my = std::min(my,irrPos2.Y * scaling.Y);
				My = std::max(My,irrPos2.Y * scaling.Y);
				mz = std::min(mz,irrPos2.Z * scaling.Z);
				Mz = std::max(Mz,irrPos2.Z * scaling.Z);
			}
		}
	}

	mOffset->X = (mx+Mx)/2.f;
	mOffset->Y = (my+My)/2.f;
	mOffset->Z = (mz+Mz)/2.f;

	f32 ccdThreshold = std::min(Mx-mx,std::min(My-my,Mz-mz));

	for (i=0; i<bufferCount; i++) { //set offset to vertices
		scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);
		if (mb->getVertexType()==video::EVT_STANDARD) {
			video::S3DVertex* mb_vertices=(video::S3DVertex*)mb->getVertices();
			numVertices = mb->getVertexCount()-precisionLoss+1;

			for (k=0;k<numVertices;k+=precisionLoss) {
				core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-mOffset->X, (irrPos2.Y * scaling.Y)-mOffset->Y, (irrPos2.Z * scaling.Z)-mOffset->Z);
				mShape->addPoint(btPos2);
			}
		} else if(mb->getVertexType()==video::EVT_2TCOORDS) {
			video::S3DVertex2TCoords* mb_vertices=(video::S3DVertex2TCoords*)mb->getVertices();
			numVertices = mb->getVertexCount()-precisionLoss+1;

			for (k=0;k<numVertices;k+=precisionLoss) {
				core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-mOffset->X, (irrPos2.Y * scaling.Y)-mOffset->Y, (irrPos2.Z * scaling.Z)-mOffset->Z);
				mShape->addPoint(btPos2);
			}
		} else if(mb->getVertexType()==video::EVT_TANGENTS) {
			video::S3DVertexTangents* mb_vertices=(video::S3DVertexTangents*)mb->getVertices();
			numVertices = mb->getVertexCount()-precisionLoss+1;

			for (k=0;k<numVertices;k+=precisionLoss) {
				core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-mOffset->X, (irrPos2.Y * scaling.Y)-mOffset->Y, (irrPos2.Z * scaling.Z)-mOffset->Z);
				mShape->addPoint(btPos2);
			}
		}
	}

	// Add mass
	btVector3 localInertia;
	mShape->calculateLocalInertia(mass, localInertia);
	mShape->setMargin(0.1f);

	btRigidBody* rbody = new btRigidBody(mass, MotionState, mShape, localInertia);
	world->addRigidBody(rbody,group,mask);

	rbody->setDamping((1.0/mass)*1.0,(1.0/mass)*1.0);
	rbody->setFriction(mass*0.001);
	rbody->setRollingFriction(mass);
	rbody->setCcdMotionThreshold(ccdThreshold);
	rbody->setCcdSweptSphereRadius(ccdThreshold*0.5f);

	objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rbody));
	offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(node, mOffset));

	return rbody;
}

void irrDynamics::registerNewRBody(irr::scene::ISceneNode* node,btRigidBody* rbody,f32 mass,short group,short mask,irr::core::vector3df nOffset) {
    world->addRigidBody(rbody,group,mask);

	if (mass>0.f) {
		rbody->setDamping((1.0/mass)*1.0,(1.0/mass)*1.0);
		rbody->setFriction(mass*0.001);
		rbody->setRollingFriction(mass);
	}

    irr::core::vector3df* mOffset = new irr::core::vector3df(nOffset);
    *mOffset = nOffset;
	objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rbody));
	offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(node, mOffset));
}

void irrDynamics::addTriMesh_static(irr::scene::IMeshSceneNode* node,short group,short mask) { //can be concave

	node->updateAbsolutePosition();
	core::vector3df irrPos = node->getPosition();
	btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btPos);
	core::vector3df* mOffset;
	mOffset = new core::vector3df(0.0f,0.0f,0.0f);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	irr::scene::IMesh* pMesh = node->getMesh();
	core::vector3df scaling = node->getScale();
	btVector3 vertices[3];
	u32 i,j,k,index,numIndices;
	u16* mb_indices;
	btTriangleMesh *pTriMesh = new btTriangleMesh();
	for (i=0; i<pMesh->getMeshBufferCount(); i++)
	{
		scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);
		if(mb->getVertexType()==video::EVT_STANDARD)
		{
			video::S3DVertex* mb_vertices=(video::S3DVertex*)mb->getVertices();
			mb_indices = mb->getIndices();
			//numVertices = mb->getVertexCount();
			numIndices = mb->getIndexCount();
			for(j=0;j<numIndices;j+=3)
			{
				for (k=0;k<3;k++)
				{
					index = mb_indices[j+k];
					vertices[k] = btVector3(mb_vertices[index].Pos.X * scaling.X,
					mb_vertices[index].Pos.Y * scaling.Y,
					mb_vertices[index].Pos.Z * scaling.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}
		else if(mb->getVertexType()==video::EVT_2TCOORDS)
		{
			video::S3DVertex2TCoords* mb_vertices=(video::S3DVertex2TCoords*)mb->getVertices();
			mb_indices = mb->getIndices();
			//numVertices = mb->getVertexCount();
			numIndices = mb->getIndexCount();
			for(j=0;j<numIndices;j+=3)
			{
				for (k=0;k<3;k++)
				{
					index = mb_indices[j+k];
					vertices[k] = btVector3(mb_vertices[index].Pos.X * scaling.X,
					mb_vertices[index].Pos.Y * scaling.Y,
					mb_vertices[index].Pos.Z * scaling.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}
	}
	//Add the mesh collision shape to the world
	btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(pTriMesh, true);
	btRigidBody* rbody = new btRigidBody(0.f, MotionState, mShape);
	world->addRigidBody(rbody,group,mask);

	objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rbody));
	offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(node, mOffset));
}

void irrDynamics::addTerrain(scene::ITerrainSceneNode* terrain, u32 lodLevel)
{

    terrain->updateAbsolutePosition();
    core::vector3df irrPos = terrain->getPosition();
    btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btPos);
    core::vector3df* mOffset;
    mOffset = new core::vector3df(0.0f,0.0f,0.0f);

    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

    btTriangleMesh * mTriMesh = new btTriangleMesh();
    scene::CDynamicMeshBuffer* buffer = 0;
    btVector3 vertices[3];
    s32 j, k;
    buffer = new scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_32BIT);
    (terrain)->getMeshBufferForLOD(*buffer, lodLevel);

    core::vector3df terrScale = terrain->getScale();
    //Build the triangleMesh
    const u32 indexCount = buffer->getIndexCount();
    printf("indeces: %u\n", indexCount);
    video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*) buffer->getVertexBuffer().getData();
    u32* mb_indices = (u32*) buffer->getIndices();
    for (j = 0; (u32) j < indexCount; j += 3)
    {
        for (k = 0; k < 3; k++)
        {
            s32 index = mb_indices[j + k];
            vertices[k] = btVector3(mb_vertices[index].Pos.X * terrScale.X, mb_vertices[index].Pos.Y * terrScale.Y, mb_vertices[index].Pos.Z * terrScale.Z);
        }
        mTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
    }

    buffer->drop();
    //Add the terrain collision shape to the world
    btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(mTriMesh, true);
    btRigidBody* rbody = new btRigidBody(0.f, MotionState, mShape);
    world->addRigidBody(rbody);

    objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(terrain, rbody));
    offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(terrain, mOffset));

    terrain->grab();
}

void irrDynamics::setGravity(f32 newGravity)
{
    world->setGravity(btVector3(0.f, newGravity, 0.f));
}

void irrDynamics::updateObjects()
{
    std::map<scene::ISceneNode*, core::vector3df*>::iterator iter2 = offset.begin();
    for (std::map<scene::ISceneNode*, btRigidBody*>::iterator iter = objects.begin(); iter != objects.end() && iter2 != offset.end(); iter++)
    {
		btTransform Transform;
        //if (iter->second->getLinearVelocity()!=btVector3(0,0,0) || iter->second->getAngularVelocity()!=btVector3(0,0,0)) {
		iter->second->getMotionState()->getWorldTransform(Transform);
		btVector3 Point = Transform.getOrigin();

		// Set rotation
		core::vector3df euler;
		const btQuaternion& quat = Transform.getRotation();
		core::quaternion q(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
		q.toEuler(euler);

		euler *= core::RADTODEG;

		iter->first->setRotation(euler);

		core::vector3df dir = *iter2->second;
		core::matrix4 rotMatrix;
		rotMatrix.setRotationDegrees(euler);
		rotMatrix.transformVect(dir);
		Point -= btVector3(dir.X,dir.Y,dir.Z);
		iter->first->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));
        //}
        iter2++;
    }
}

void irrDynamics::removeObject(scene::ISceneNode* node)
{

    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter = objects.find(node);
    std::map<scene::ISceneNode*, core::vector3df*>::iterator iter2 = offset.find(node);
    if (iter != objects.end())
    {
        removeConstraints(iter->second);
        world->removeRigidBody(iter->second);

        // Free memory
        delete iter->second->getMotionState();
        delete iter->second->getCollisionShape();
        delete iter->second;
        delete iter2->second;
        iter->first->drop();
        objects.erase(iter);
        offset.erase(iter2);
    }
    else
    {
        printf("irrdynamics: object not found in map!\n");
    }
}

void irrDynamics::unregisterRBody(btRigidBody* rbody) {
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter = objects.begin();
    std::map<scene::ISceneNode*, core::vector3df*>::iterator iter2 = offset.begin();

    while (iter!=objects.end() && iter->second!=rbody) {
        iter++; iter2++;
    }

    if (iter != objects.end())
    {
        removeConstraints(iter->second);
        world->removeRigidBody(iter->second);

        // Free memory
        //delete iter->second->getMotionState();
        //delete iter->second->getCollisionShape();
        //delete iter->second;
        delete iter2->second;
        //iter->first->drop();
        objects.erase(iter);
        offset.erase(iter2);
    }
}

void irrDynamics::clearObjects()
{
    std::map<scene::ISceneNode*, core::vector3df*>::iterator iter2 = offset.begin();
    for (std::map<scene::ISceneNode*, btRigidBody*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
    {
        removeConstraints(iter->second);
        world->removeRigidBody(iter->second);

        // Free memory
        delete iter->second->getMotionState();
        delete iter->second->getCollisionShape();
        delete iter->second;
        delete iter2->second;
        iter->first->drop();
        iter2++;
    }
    objects.clear();
    offset.clear();
}

btRigidBody* irrDynamics::addSphericalObject(scene::ISceneNode* node, f32 radius, f32 mass,short group,short mask)
{

    node->updateAbsolutePosition();
    core::vector3df irrPos = node->getAbsolutePosition();
    btVector3 tPosition(irrPos.X, irrPos.Y, irrPos.Z);
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(tPosition);
    core::vector3df* mOffset;
    mOffset = new core::vector3df(0.0f,0.0f,0.0f);

    btDefaultMotionState *motionState = new btDefaultMotionState(Transform);

    // Create the shape
    btCollisionShape *shape = new btSphereShape(radius);

    // Add mass
    btVector3 localInertia;
    shape->calculateLocalInertia(mass, localInertia);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(mass, motionState, shape, localInertia);

    // Add it to the world
    world->addRigidBody(rigidBody,group,mask);

    rigidBody->setDamping((1.0/mass)*1.0,(1.0/mass)*1.0);
    rigidBody->setFriction(mass*0.001);

    objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rigidBody));
    offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(node, mOffset));
    //node->grab();

    rigidBody->activate(true);
    //rigidBody->applyCentralImpulse( btVector3( 40.f, 100.f, -50.f ) );

    return rigidBody;
}

bool irrDynamics::rayTest(btVector3 Start,btVector3 End) {
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	world->rayTest(Start,End,RayCallback);

	return RayCallback.hasHit();
}

irr::core::vector3df irrDynamics::rayTestPoint(class btVector3 Start,class btVector3 End) {
    btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	world->rayTest(Start,End,RayCallback);

    if (RayCallback.hasHit()) {
        return irr::core::vector3df(RayCallback.m_hitPointWorld[0],RayCallback.m_hitPointWorld[1],RayCallback.m_hitPointWorld[2]);
    } else {
        return irr::core::vector3df(Start[0],Start[1],Start[2]);
    }
}

btRigidBody* irrDynamics::addPlayerColliderObject(scene::ISceneNode* node, f32 height, f32 radius, f32 mass,short group,short mask)
{

    node->updateAbsolutePosition();
    core::vector3df irrPos = node->getAbsolutePosition();
    btVector3 tPosition(irrPos.X, irrPos.Y, irrPos.Z);
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(tPosition);
    core::vector3df* mOffset;
    mOffset = new core::vector3df(0.0f,-(height/2.0)+(radius/1.5),0.0f);

    btDefaultMotionState *motionState = new btDefaultMotionState(Transform);

    // Create the shape
    btConvexHullShape *mShape = new btConvexHullShape();

	f32 oRadius = radius;
	height-=oRadius*0.1f;
	radius-=oRadius*0.1f;
	for (int i=0;i<90;i++) {
		f32 fi = (f32)i*4.f*irr::core::DEGTORAD;
		mShape->addPoint(btVector3(std::cos(fi)*0.1f*radius,-height*0.5f,std::sin(fi)*0.1f*radius));
		mShape->addPoint(btVector3(std::cos(fi)*radius,-height*0.5f+radius*1.1f,std::sin(fi)*radius));
		mShape->addPoint(btVector3(std::cos(fi)*radius,height*0.5f,std::sin(fi)*radius));
	}
	mShape->setMargin(oRadius*0.02f);
	//mShape->initializePolyhedralFeatures();

    // Add mass
    btVector3 localInertia;
    mShape->calculateLocalInertia(mass, localInertia);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(mass, motionState, mShape, localInertia);
    rigidBody->setRestitution(0.0f);
    rigidBody->setContactProcessingThreshold(btScalar(4.0f));

    // Add it to the world
    world->addRigidBody(rigidBody,group,mask);

    rigidBody->setDamping((1.0/mass)*0.01,(1.0/mass)*0.01);
    rigidBody->setFriction(mass*0.001);
    rigidBody->setRollingFriction(mass*0.001);

    objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rigidBody));
    offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(node, mOffset));
    //node->grab();

    rigidBody->activate(true);
    //rigidBody->applyCentralImpulse( btVector3( 40.f, 100.f, -50.f ) );

    return rigidBody;
}

btRigidBody* irrDynamics::addCapsuleObject(scene::ISceneNode* node, f32 height, f32 radius, f32 mass,short group,short mask)
{

    node->updateAbsolutePosition();
    core::vector3df irrPos = node->getAbsolutePosition();
    btVector3 tPosition(irrPos.X, irrPos.Y, irrPos.Z);
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(tPosition);
    core::vector3df* mOffset;
    mOffset = new core::vector3df(0.0f,-height/2.0,0.0f);
    height-=radius*2;

    btDefaultMotionState *motionState = new btDefaultMotionState(Transform);

    // Create the shape
    btCollisionShape *shape = new btCapsuleShape(radius,height);

    // Add mass
    btVector3 localInertia;
    shape->calculateLocalInertia(mass, localInertia);
    shape->setMargin(0.1f);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(mass, motionState, shape, localInertia);

    // Add it to the world
    world->addRigidBody(rigidBody,group,mask);

    rigidBody->setDamping((1.0/mass)*1.0,(1.0/mass)*1.0);
    rigidBody->setFriction(mass*0.001);
    rigidBody->setRollingFriction(mass);

    objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rigidBody));
    offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(node, mOffset));
    //node->grab();

    rigidBody->activate(true);
    //rigidBody->applyCentralImpulse( btVector3( 100.f, 1000.f, -700.f ) );

    return rigidBody;
}

btRigidBody* irrDynamics::addBoxObject(scene::ISceneNode* node, f32 mass,short group,short mask)
{

    node->updateAbsolutePosition();
    core::vector3df irrPos = node->getAbsolutePosition();
    btVector3 tPosition(irrPos.X, irrPos.Y, irrPos.Z);



    const core::aabbox3df aabox = node->getTransformedBoundingBox();
    core::vector3df fullExtent = aabox.getExtent();
    btVector3 halfExtent(fullExtent.X * .5f, fullExtent.Y * .5f, fullExtent.Z * .5f);

    //get rotation to quaternion
    core::vector3df rotationRadians = node->getRotation();
    rotationRadians *= core::DEGTORAD;
    core::quaternion quat(rotationRadians);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(tPosition);
    transform.setRotation(btQuaternion(quat.X, quat.Y, quat.Z, quat.W));
    core::vector3df* mOffset;
    mOffset = new core::vector3df(0.0f,0.0f,0.0f);

    btDefaultMotionState *motionState = new btDefaultMotionState(transform);

    // Create the shape
    btCollisionShape *shape = new btBoxShape(halfExtent);

    // Add mass
    btVector3 localInertia;
    shape->calculateLocalInertia(mass, localInertia);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(mass, motionState, shape, localInertia);

    // Add it to the world
    world->addRigidBody(rigidBody,group,mask);

    if (mass>0.f) {
        rigidBody->setDamping((1.0/mass)*1.0,(1.0/mass)*1.0);
        rigidBody->setFriction(mass*0.001);
        rigidBody->setRollingFriction(mass);
    }

    objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rigidBody));
    offset.insert (std::pair<scene::ISceneNode*, core::vector3df*>(node, mOffset));
    node->grab();
    return rigidBody;
}

bool irrDynamics::createHingeConstraint(scene::ISceneNode* nodeA, scene::ISceneNode* nodeB, const core::vector3df& pivotInA, const core::vector3df& pivotInB, const core::vector3df& axisInA, const irr::core::vector3df& axisInB)
{

    //find the corresponding rigid bodies:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iterA, iterB;
    iterA = objects.find(nodeA);
    iterB = objects.find(nodeB);

    if (iterA == objects.end())
    {
        printf("irrdynamics: Unable to find first node for constraint!\n");
        return false;
    }

    if (iterB == objects.end())
    {
        printf("irrdynamics: Unable to find second node for constraint!\n");
        return false;
    }

    btHingeConstraint* constraint = new btHingeConstraint(*(iterA->second), *(iterB->second), btVector3(pivotInA.X, pivotInA.Y, pivotInA.Z), btVector3(pivotInB.X, pivotInB.Y, pivotInB.Z), btVector3(axisInA.X, axisInA.Y, axisInA.Z), btVector3(axisInB.X, axisInB.Y, axisInB.Z));
    world->addConstraint(constraint);
    iterA->second->addConstraintRef(constraint);
    iterB->second->addConstraintRef(constraint);
    return true;
}

bool irrDynamics::createPoint2PointConstraint(scene::ISceneNode* nodeA, scene::ISceneNode* nodeB, const core::vector3df& pivotInA, const core::vector3df& pivotInB)
{

    //find the corresponding rigid bodies:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iterA, iterB;
    iterA = objects.find(nodeA);
    iterB = objects.find(nodeB);

    if (iterA == objects.end())
    {
        printf("irrdynamics: Unable to find first node for constraint!\n");
        return false;
    }

    if (iterB == objects.end())
    {
        printf("irrdynamics: Unable to find second node for constraint!\n");
        return false;
    }

    btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*(iterA->second), *(iterB->second), btVector3(pivotInA.X, pivotInA.Y, pivotInA.Z), btVector3(pivotInB.X, pivotInB.Y, pivotInB.Z));
    world->addConstraint(constraint);
    iterA->second->addConstraintRef(constraint);
    iterB->second->addConstraintRef(constraint);
    return true;
}

bool irrDynamics::createSliderConstraint(scene::ISceneNode* nodeA,
                                         scene::ISceneNode* nodeB,
                                         const core::vector3df& posInA,
                                         const core::vector3df& rotInA,
                                         const core::vector3df& posInB,
                                         const core::vector3df& rotInB)
{

    //find the corresponding rigid bodies:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iterA, iterB;
    iterA = objects.find(nodeA);
    iterB = objects.find(nodeB);

    if (iterA == objects.end())
    {
        printf("irrdynamics: Unable to find first node for constraint!\n");
        return false;
    }

    if (iterB == objects.end())
    {
        printf("irrdynamics: Unable to find second node for constraint!\n");
        return false;
    }

    btTransform matA, matB;
    core::vector3df rotationRadians = rotInA;
    rotationRadians *= core::DEGTORAD;
    core::quaternion rotA(rotationRadians);

    rotationRadians = rotInB;
    rotationRadians *= core::DEGTORAD;
    core::quaternion rotB(rotationRadians);

    matA.setIdentity();
    matA.setOrigin(btVector3(posInA.X, posInA.Y, posInA.Z));
    matA.setRotation(btQuaternion(rotA.X, rotA.Y, rotA.Z, rotA.W));

    matB.setIdentity();
    matB.setOrigin(btVector3(posInB.X, posInB.Y, posInB.Z));
    matB.setRotation(btQuaternion(rotB.X, rotB.Y, rotB.Z, rotB.W));


    btSliderConstraint* constraint = new btSliderConstraint(*(iterA->second), *(iterB->second), matA, matB, true);
    world->addConstraint(constraint);
    iterA->second->addConstraintRef(constraint);
    iterB->second->addConstraintRef(constraint);
    return true;
}

void irrDynamics::applyCentralForce(scene::ISceneNode* node, const core::vector3df& force)
{

    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = objects.find(node);
    if (iter == objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Force application aborted.\n");
        return;
    }

    iter->second->applyCentralForce(btVector3(force.X, force.Y, force.Z));
    iter->second->activate();
}

void irrDynamics::applyCentralImpulse(scene::ISceneNode* node, const core::vector3df& force)
{

    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = objects.find(node);
    if (iter == objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Impulse application aborted.\n");
        return;
    }

    iter->second->applyCentralImpulse(btVector3(force.X, force.Y, force.Z));
    iter->second->activate();
}

void irrDynamics::applyTorque(scene::ISceneNode* node, const core::vector3df& torque)
{

    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = objects.find(node);
    if (iter == objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Torque application aborted.\n");
        return;
    }

    iter->second->applyTorque(btVector3(torque.X, torque.Y, torque.Z));
    iter->second->activate();
}

btRigidBody* irrDynamics::addFloor(const core::vector3df& normal, const core::vector3df& offset)
{

    btVector3 tPosition(offset.X, offset.Y, offset.Z);
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(tPosition);
    btDefaultMotionState *motionState = new btDefaultMotionState(transform);

    // Create the shape
    btCollisionShape *shape = new btStaticPlaneShape(btVector3(normal.X, normal.Y, normal.Z), 0);

    // Add mass
    btVector3 localInertia;
    shape->calculateLocalInertia(0.f, localInertia);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(0.f, motionState, shape, localInertia);

    // Add it to the world
    world->addRigidBody(rigidBody);

    return rigidBody;
}

void irrDynamics::setDamping(scene::ISceneNode* node, f32 linearDamping, f32 angularDamping)
{

    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = objects.find(node);
    if (iter == objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Damping application aborted.\n");
        return;
    }

    iter->second->setDamping(linearDamping, angularDamping);
}

void irrDynamics::setPosition(scene::ISceneNode* node, const core::vector3df& newPos)
{

    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = objects.find(node);
    if (iter == objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Position update aborted.\n");
        return;
    }

    btTransform transform = iter->second->getCenterOfMassTransform();
    transform.setOrigin(btVector3(newPos.X, newPos.Y, newPos.Z));
    iter->second->setCenterOfMassTransform(transform);
}

void irrDynamics::removeConstraints(btRigidBody* rigidBody)
{
    const int len = rigidBody->getNumConstraintRefs();
    //first, remove constraint reference at the other node
    for (int i = 0; i < len; i++)
    {
        btTypedConstraint* constraint = rigidBody->getConstraintRef(i);
        btRigidBody* bodyB = &(constraint->getRigidBodyB());
        if (bodyB == rigidBody)
            bodyB = &(constraint->getRigidBodyA());

        bodyB->removeConstraintRef(constraint);
    }
    //then remove/delete at our side
    while (rigidBody->getNumConstraintRefs() > 0)
    {
        btTypedConstraint* constraint = rigidBody->getConstraintRef(0);
        rigidBody->removeConstraintRef(constraint);
        world->removeConstraint(constraint);
        delete constraint;
        printf("DELETED\n");
    }
}
