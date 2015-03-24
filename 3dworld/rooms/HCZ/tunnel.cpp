#include "../room.h"
#include "tunnel.h"

RMesh* tunnel::baseRMesh = nullptr;

void tunnel::setBase(RMesh* inRme) {
	if (tunnel::baseRMesh==nullptr) {
		tunnel::baseRMesh = inRme;
	}
}

tunnel* tunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	tunnel* retRoom = new tunnel;

	retRoom->loadAssets(tunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void tunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(tunnel::baseRMesh,startPos,destPos,posList);
}
