#include "Room.h"

#include <cmath>

#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>

/*bool CustomMaterialCombinerCallback(btManifoldPoint& cp,const btCollisionObjectWrapper* colObj0,int partId0,int index0,const btCollisionObjectWrapper* colObj1,int partId1,int index1)
{
	std::cout<<"called custom callback\n";
	btAdjustInternalEdgeContacts(cp,colObj1,colObj0, partId1,index1);

	float friction0 = colObj0->getCollisionObject()->getFriction();
	float friction1 = colObj1->getCollisionObject()->getFriction();
	float restitution0 = colObj0->getCollisionObject()->getRestitution();
	float restitution1 = colObj1->getCollisionObject()->getRestitution();

	if (colObj0->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		friction0 = 1.0;//partId0,index0
		restitution0 = 0.f;
	}
	if (colObj1->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		friction1 = 1.0f;//partId1,index1
		restitution1 = 0.f;
	}

	cp.m_combinedFriction = friction0*friction1;
	cp.m_combinedRestitution = restitution0*restitution1;

	//this return value is currently ignored, but to be on the safe side: return false if you don't calculate friction
	return true;
}

extern ContactAddedCallback gContactAddedCallback;*/

irrDynamics* Room::dynamics = nullptr;
irr::scene::ISceneManager* Room::smgr = nullptr;

Room::Room() {
    std::cout<<"Room constructor\n";
	for (unsigned char i=0;i<4;i++) {
		linkedTurnDists[i]=-1;
	}
}

void Room::setDynamics(irrDynamics* inDyn) {
	if (Room::dynamics==nullptr) Room::dynamics = inDyn;
}

void Room::setSmgr(irr::scene::ISceneManager* inSmgr) {
	if (Room::smgr==nullptr) Room::smgr = inSmgr;
}

irr::scene::ITriangleSelector* Room::getSelector(irr::scene::IMesh* mesh) {
	if (node->getTriangleSelector()==nullptr) {
		irr::scene::ITriangleSelector* retSelec = smgr->createTriangleSelector(mesh,node);
		node->setTriangleSelector(retSelec);
		retSelec->drop();
		return retSelec;
	}
	return node->getTriangleSelector();
}

irr::scene::IMeshSceneNode* Room::getNewNode(irr::scene::IMesh* mesh) {
	return smgr->addMeshSceneNode(mesh);
}

void Room::setActivation(bool s) {
	if (s!=isActivated) {
		if (s) {
			dynamics->registerNewRBody(node,rbody,-1,~0,~0,irr::core::vector3df(0,0.f,0));
			node->setVisible(true);
		} else {
			dynamics->unregisterRBody(rbody);
			node->setVisible(false);
		}
		isActivated = s;
	}
}

void Room::loadAssets(RMesh* rme,irr::core::vector3df inPosition,float inAngle) {
	node = Room::getNewNode(rme->mesh); node->setVisible(true);
	getSelector(rme->mesh);
	node->setPosition(inPosition);
	node->setRotation(irr::core::vector3df(0,inAngle*90.f,0));

	angle = inAngle;

	//Add the Bullet rigid body
	node->updateAbsolutePosition();
	irr::core::vector3df irrPos = node->getAbsolutePosition();
	btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btPos);
	btQuaternion btRot;
	irr::core::vector3df irrRot = node->getRotation();
	btRot.setEulerZYX(irrRot.Z*irr::core::DEGTORAD,irrRot.Y*irr::core::DEGTORAD,irrRot.X*irr::core::DEGTORAD);
	Transform.setRotation(btRot);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	btVector3 localInertia;
	rme->shape->calculateLocalInertia(0.0, localInertia);

	rbody = new btRigidBody(0.0, MotionState, rme->shape, localInertia);
	Room::dynamics->registerNewRBody(node,rbody,-1,~0,~0,irr::core::vector3df(0,0.f,0));

	rotMatrix.setRotationDegrees(irr::core::vector3df(0,inAngle*90.f,0));

	for (unsigned int i=0;i<rme->pointlights.size();i++) {
        pointLight lght = rme->pointlights[i];
        rotMatrix.transformVect(lght.position);
        lght.position+=inPosition;

        lght.viewMatrix[0].buildCameraLookAtMatrixLH(lght.position,lght.position+irr::core::vector3df(0.f,0.f,1.f),irr::core::vector3df(0.f,1.f,0.f));
        lght.viewMatrix[1].buildCameraLookAtMatrixLH(lght.position,lght.position+irr::core::vector3df(1.f,0.f,0.f),irr::core::vector3df(0.f,1.f,0.f));
        lght.viewMatrix[2].buildCameraLookAtMatrixLH(lght.position,lght.position+irr::core::vector3df(0.f,0.f,-1.f),irr::core::vector3df(0.f,1.f,0.f));
        lght.viewMatrix[3].buildCameraLookAtMatrixLH(lght.position,lght.position+irr::core::vector3df(-1.f,0.f,0.f),irr::core::vector3df(0.f,1.f,0.f));
        lght.viewMatrix[4].buildCameraLookAtMatrixLH(lght.position,lght.position+irr::core::vector3df(0.f,1.f,0.f),irr::core::vector3df(0.f,0.f,1.f));
        lght.viewMatrix[5].buildCameraLookAtMatrixLH(lght.position,lght.position+irr::core::vector3df(0.f,-1.f,0.f),irr::core::vector3df(0.f,0.f,1.f));

        if (lght.viewMatrix[0].equals(lght.viewMatrix[5])) { std::terminate(); }

        pointLights.push_back(lght);
	}

	/*for (unsigned int i=0;i<rme->waypoints.size();i++) {
        irr::core::vector3df pos = rme->waypoints[i]->position;
        rotMatrix.transformVect(pos);
        //pos*=0.75f*0.1f;
        irr::scene::ISceneNode* cube = Room::smgr->addCubeSceneNode(1.f);
        cube->getMaterial(0).Lighting=false;
        cube->setPosition(pos+inPosition);
	}*/

	rbody->setFriction(1.f);
	rbody->setRollingFriction(1.f);
	rbody->setContactProcessingThreshold(1.f);

	rbody->setCollisionFlags(rbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

void Room::destroy() {
	node->drop();
	dynamics->unregisterRBody(rbody);
	delete rbody;
}

void Room::setLinkedTurnDist(unsigned char index,signed char value) {
	index = index % 4;
    //if (linkedTurnDists[index]<=0) {
    linkedTurnDists[index]=value;
    //}
}

char Room::getLinkedTurnDist(unsigned char index) const {
	index = index % 4;
	return linkedTurnDists[index];
}

const std::vector<pointLight>& Room::getPointLights() {
    return pointLights;
}
