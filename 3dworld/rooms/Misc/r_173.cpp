#include "../room.h"
#include "r_173.h"

RMesh* r_173::baseRMesh = nullptr;

void r_173::setBase(RMesh* inRme) {
	if (r_173::baseRMesh==nullptr) {
		r_173::baseRMesh = inRme;
	}
}

r_173* r_173::createNew(irr::core::vector3df inPosition,char inAngle) {
	r_173* retRoom = new r_173;

	retRoom->node = room::getNewNode(r_173::baseRMesh->mesh); retRoom->node->setVisible(true);
	retRoom->getSelector(r_173::baseRMesh->mesh);
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
	r_173::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, r_173::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& r_173::getPointLights() {
	return r_173::baseRMesh->pointlights;
}
