#include "../room.h"
#include "room2ctunnel.h"

irr::scene::IMeshSceneNode* room2ctunnel::baseNode = nullptr;
btBvhTriangleMeshShape* room2ctunnel::baseShape = nullptr;

void room2ctunnel::setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape) {
	if (room2ctunnel::baseNode==nullptr || room2ctunnel::baseShape==nullptr) {
		room2ctunnel::baseNode = inNode; room2ctunnel::baseShape = inShape; room2ctunnel::baseNode->setVisible(false);
	}
}

room2ctunnel* room2ctunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2ctunnel* retRoom = new room2ctunnel;
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
	room2ctunnel::baseShape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, room2ctunnel::baseShape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}
