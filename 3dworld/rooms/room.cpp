#include "room.h"

#include <cmath>

dynRegister* room::dynamics = nullptr;
irr::scene::ISceneManager* room::smgr = nullptr;

void room::setDynamics(dynRegister* inDyn) {
	if (room::dynamics==nullptr) room::dynamics = inDyn;
}

void room::setSmgr(irr::scene::ISceneManager* inSmgr) {
	if (room::smgr==nullptr) room::smgr = inSmgr;
}

irr::scene::ITriangleSelector* room::getSelector(irr::scene::IMesh* mesh) {
	if (node->getTriangleSelector()==nullptr) {
		irr::scene::ITriangleSelector* retSelec = smgr->createOctreeTriangleSelector(mesh,node,150);
		node->setTriangleSelector(retSelec);
		retSelec->drop();
		return retSelec;
	}
	return node->getTriangleSelector();
}

irr::scene::IMeshSceneNode* room::getNewNode(irr::scene::IMesh* mesh) {
	return smgr->addOctreeSceneNode(mesh);
}

void room::setActivation(bool s) {
	if (s!=isActivated) {
		if (s) {
			dynamics->sharedRegisterRBody(node,rbody,-1);
			node->setVisible(true);
		} else {
			dynamics->sharedUnregisterRBody(rbody);
			node->setVisible(false);
		}
		isActivated = s;
	}
}

void room::loadAssets(RMesh* rme,irr::core::vector3df inPosition,float inAngle) {
	node = room::getNewNode(rme->mesh); node->setVisible(true);
	getSelector(rme->mesh);
	node->setPosition(inPosition);
	node->setRotation(irr::core::vector3df(0,inAngle*90.f,0));
	angle = inAngle;

	//Add the Bullet rigid body
	node->updateAbsolutePosition();
	irr::core::vector3df irrPos = node->getAbsolutePosition();
	btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btPos);
	btQuaternion btRot;
	irr::core::vector3df irrRot = node->getRotation();
	btRot.setEulerZYX(irrRot.Z*irr::core::DEGTORAD,irrRot.Y*irr::core::DEGTORAD,irrRot.X*irr::core::DEGTORAD);
	Transform.setRotation(btRot);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	btVector3 localInertia;
	rme->shape->calculateLocalInertia(0.0, localInertia);

	rbody = new btRigidBody(0.0, MotionState, rme->shape, localInertia);
	room::dynamics->sharedRegisterRBody(node,rbody,0.f);

	rbody->setFriction(1.f);
	rbody->setRollingFriction(1.f);
}
