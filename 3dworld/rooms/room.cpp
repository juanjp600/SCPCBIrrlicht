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
