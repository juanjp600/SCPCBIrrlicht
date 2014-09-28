#include "../room.h"
#include "gateaentrance.h"

irr::scene::IMeshSceneNode* gateaentrance::baseNode = nullptr;
RMesh* gateaentrance::baseRMesh = nullptr;

void gateaentrance::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {
	if (gateaentrance::baseNode==nullptr || gateaentrance::baseRMesh==nullptr) {
		gateaentrance::baseNode = inNode; gateaentrance::baseRMesh = inRme; gateaentrance::baseNode->setVisible(false);
	}
}

gateaentrance* gateaentrance::createNew(irr::core::vector3df inPosition,char inAngle) {
	gateaentrance* retRoom = new gateaentrance;

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
	gateaentrance::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);

	retRoom->rbody = new btRigidBody(0.0, MotionState, gateaentrance::baseRMesh->shape, localInertia);
	room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

	retRoom->rbody->setFriction(1.f);
	retRoom->rbody->setRollingFriction(1.f);
	return retRoom;
}

const std::vector<irr::video::SLight>& gateaentrance::getPointLights() {
	return gateaentrance::baseRMesh->pointlights;
}
