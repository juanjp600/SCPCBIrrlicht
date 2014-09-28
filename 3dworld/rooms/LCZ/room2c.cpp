#include "../room.h"
#include "room2c.h"

irr::scene::IMeshSceneNode* room2c::baseNode = nullptr;
RMesh* room2c::baseRMesh = nullptr;

void room2c::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (room2c::baseNode==nullptr || room2c::baseRMesh==nullptr) {
		room2c::baseNode = inNode; room2c::baseRMesh = inRme; room2c::baseNode->setVisible(false);
	}
}

room2c* room2c::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2c* retRoom = new room2c;

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
	room2c::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room2c::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2c::getPointLights() {
	return room2c::baseRMesh->pointlights;
}
