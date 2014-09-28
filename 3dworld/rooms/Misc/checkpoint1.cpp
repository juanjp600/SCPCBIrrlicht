#include "../room.h"
#include "checkpoint1.h"

irr::scene::IMeshSceneNode* checkpoint1::baseNode = nullptr;
RMesh* checkpoint1::baseRMesh = nullptr;

void checkpoint1::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (checkpoint1::baseNode==nullptr || checkpoint1::baseRMesh==nullptr) {
		checkpoint1::baseNode = inNode; checkpoint1::baseRMesh = inRme; checkpoint1::baseNode->setVisible(false);
	}
}

checkpoint1* checkpoint1::createNew(irr::core::vector3df inPosition,char inAngle) {
	checkpoint1* retRoom = new checkpoint1;

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
	checkpoint1::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, checkpoint1::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}
