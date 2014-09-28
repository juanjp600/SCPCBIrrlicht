#include "../room.h"
#include "tunnel2.h"

RMesh* tunnel2::baseRMesh = nullptr;

void tunnel2::setBase(RMesh* inRme) {
	if (tunnel2::baseRMesh==nullptr) {
		tunnel2::baseRMesh = inRme;
	}
}

tunnel2* tunnel2::createNew(irr::core::vector3df inPosition,char inAngle) {
	tunnel2* retRoom = new tunnel2;

	retRoom->node = room::getNewNode(tunnel2::baseRMesh->mesh); retRoom->node->setVisible(true);
	retRoom->getSelector(tunnel2::baseRMesh->mesh);
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
	tunnel2::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, tunnel2::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& tunnel2::getPointLights() {
	return tunnel2::baseRMesh->pointlights;
}
