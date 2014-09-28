#include "../room.h"
#include "pocketdimension.h"

irr::scene::IMeshSceneNode* pocketdimension::baseNode = nullptr;
RMesh* pocketdimension::baseRMesh = nullptr;

void pocketdimension::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (pocketdimension::baseNode==nullptr || pocketdimension::baseRMesh==nullptr) {
		pocketdimension::baseNode = inNode; pocketdimension::baseRMesh = inRme; pocketdimension::baseNode->setVisible(false);
	}
}

pocketdimension* pocketdimension::createNew(irr::core::vector3df inPosition,char inAngle) {
	pocketdimension* retRoom = new pocketdimension;

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
	pocketdimension::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, pocketdimension::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& pocketdimension::getPointLights() {
	return pocketdimension::baseRMesh->pointlights;
}
