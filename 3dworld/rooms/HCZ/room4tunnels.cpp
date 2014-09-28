#include "../room.h"
#include "room4tunnels.h"

irr::scene::IMeshSceneNode* room4tunnels::baseNode = nullptr;
RMesh* room4tunnels::baseRMesh = nullptr;

void room4tunnels::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (room4tunnels::baseNode==nullptr || room4tunnels::baseRMesh==nullptr) {
		room4tunnels::baseNode = inNode; room4tunnels::baseRMesh = inRme; room4tunnels::baseNode->setVisible(false);
	}
}

room4tunnels* room4tunnels::createNew(irr::core::vector3df inPosition,char inAngle) {
	room4tunnels* retRoom = new room4tunnels;

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
	room4tunnels::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room4tunnels::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& room4tunnels::getPointLights() {
	return room4tunnels::baseRMesh->pointlights;
}
