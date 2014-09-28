#include "../room.h"
#include "room012.h"

irr::scene::IMeshSceneNode* room012::baseNode = nullptr;
RMesh* room012::baseRMesh = nullptr;

void room012::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (room012::baseNode==nullptr || room012::baseRMesh==nullptr) {
		room012::baseNode = inNode; room012::baseRMesh = inRme; room012::baseNode->setVisible(false);
	}
}

room012* room012::createNew(irr::core::vector3df inPosition,char inAngle) {
	room012* retRoom = new room012;

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
	room012::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room012::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& room012::getPointLights() {
	return room012::baseRMesh->pointlights;
}
