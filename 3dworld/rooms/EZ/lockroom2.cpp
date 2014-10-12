#include "../room.h"
#include "lockroom2.h"

RMesh* lockroom2::baseRMesh = nullptr;

void lockroom2::setBase(RMesh* inRme) {
	if (lockroom2::baseRMesh==nullptr) {
		lockroom2::baseRMesh = inRme;
	}
}

lockroom2* lockroom2::createNew(irr::core::vector3df inPosition,char inAngle) {
	lockroom2* retRoom = new lockroom2;

	retRoom->loadAssets(lockroom2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& lockroom2::getPointLights() {
	return lockroom2::baseRMesh->pointlights;
}
