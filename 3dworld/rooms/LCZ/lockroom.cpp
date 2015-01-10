#include "../room.h"
#include "lockroom.h"

RMesh* lockroom::baseRMesh = nullptr;

void lockroom::setBase(RMesh* inRme) {
	if (lockroom::baseRMesh==nullptr) {
		lockroom::baseRMesh = inRme;
	}
}

lockroom* lockroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	lockroom* retRoom = new lockroom;

	retRoom->loadAssets(lockroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& lockroom::getPointLights() {
	return lockroom::baseRMesh->pointlights;
}

void lockroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(lockroom::baseRMesh,startPos,destPos,posList);
}
