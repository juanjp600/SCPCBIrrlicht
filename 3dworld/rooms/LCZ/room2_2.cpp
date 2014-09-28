#include "../room.h"
#include "room2_2.h"

irr::scene::IMeshSceneNode* room2_2::baseNode = nullptr;
RMesh* room2_2::baseRMesh = nullptr;

void room2_2::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (room2_2::baseNode==nullptr || room2_2::baseRMesh==nullptr) {
		room2_2::baseNode = inNode; room2_2::baseRMesh = inRme; room2_2::baseNode->setVisible(false);
	}
}

room2_2* room2_2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2_2* retRoom = new room2_2;

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
	room2_2::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room2_2::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}
