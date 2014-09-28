#include "../room.h"
#include "room2cafeteria.h"

irr::scene::IMeshSceneNode* room2cafeteria::baseNode = nullptr;
RMesh* room2cafeteria::baseRMesh = nullptr;

void room2cafeteria::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (room2cafeteria::baseNode==nullptr || room2cafeteria::baseRMesh==nullptr) {
		room2cafeteria::baseNode = inNode; room2cafeteria::baseRMesh = inRme; room2cafeteria::baseNode->setVisible(false);
	}
}

room2cafeteria* room2cafeteria::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2cafeteria* retRoom = new room2cafeteria;

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
	room2cafeteria::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room2cafeteria::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}
