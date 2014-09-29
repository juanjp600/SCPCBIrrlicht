#include "../room.h"
#include "room2ccont.h"

RMesh* room2ccont::baseRMesh = nullptr;

void room2ccont::setBase(RMesh* inRme) {
	if (room2ccont::baseRMesh==nullptr) {
		room2ccont::baseRMesh = inRme;
	}
}

room2ccont* room2ccont::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2ccont* retRoom = new room2ccont;

	retRoom->node = room::getNewNode(room2ccont::baseRMesh->mesh); retRoom->node->setVisible(true);
	retRoom->getSelector(room2ccont::baseRMesh->mesh);
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
	room2ccont::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room2ccont::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2ccont::getPointLights() {
	return room2ccont::baseRMesh->pointlights;
}