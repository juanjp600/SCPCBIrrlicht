#include "../room.h"
#include "exit1.h"

RMesh* exit1::baseRMesh = nullptr;

void exit1::setBase(RMesh* inRme) {
	if (exit1::baseRMesh==nullptr) {
		exit1::baseRMesh = inRme;
	}
}

exit1* exit1::createNew(irr::core::vector3df inPosition,char inAngle) {
	exit1* retRoom = new exit1;

	retRoom->node = room::getNewNode(exit1::baseRMesh->mesh); retRoom->node->setVisible(true);
	retRoom->getSelector(exit1::baseRMesh->mesh);
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
	exit1::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, exit1::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& exit1::getPointLights() {
	return exit1::baseRMesh->pointlights;
}
