#include "../room.h"
#include "room3storage.h"

irr::scene::IMeshSceneNode* room3storage::baseNode = nullptr;
RMesh* room3storage::baseRMesh = nullptr;

void room3storage::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (room3storage::baseNode==nullptr || room3storage::baseRMesh==nullptr) {
		room3storage::baseNode = inNode; room3storage::baseRMesh = inRme; room3storage::baseNode->setVisible(false);
	}
}

room3storage* room3storage::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3storage* retRoom = new room3storage;

	retRoom->node = baseNode->clone(); retRoom->node->setVisible(true);
	retRoom->node->setPosition(inPosition);
	retRoom->node->setRotation(irr::core::vector3df(0,inAngle*90.f,0));
	retRoom->angle = inAngle;

	//Add the Bullet rigid body
	retRoom->node->updateAbsolutePosition();
	irr::core::vector3df irrPos = retRoom->node->getAbsolutePosition();
	btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btPos);
	btQuaternion btRot;
	irr::core::vector3df irrRot = retRoom->node->getRotation();
	btRot.setEulerZYX(irrRot.Z*irr::core::DEGTORAD,irrRot.Y*irr::core::DEGTORAD,irrRot.X*irr::core::DEGTORAD);
	Transform.setRotation(btRot);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	btVector3 localInertia;
	room3storage::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room3storage::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3storage::getPointLights() {
	return room3storage::baseRMesh->pointlights;
}
