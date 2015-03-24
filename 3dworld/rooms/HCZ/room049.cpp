#include "../room.h"
#include "room049.h"

RMesh* room049::baseRMesh = nullptr;

void room049::setBase(RMesh* inRme) {
	if (room049::baseRMesh==nullptr) {
		room049::baseRMesh = inRme;
	}
}

room049* room049::createNew(irr::core::vector3df inPosition,char inAngle) {
	room049* retRoom = new room049;

	retRoom->loadAssets(room049::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room049::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room049::baseRMesh,startPos,destPos,posList);
}
