#include "../room.h"
#include "checkpoint1.h"

RMesh* checkpoint1::baseRMesh = nullptr;

void checkpoint1::setBase(RMesh* inRme) {
	if (checkpoint1::baseRMesh==nullptr) {
		checkpoint1::baseRMesh = inRme;
	}
}

checkpoint1* checkpoint1::createNew(irr::core::vector3df inPosition,char inAngle) {
	checkpoint1* retRoom = new checkpoint1;

	retRoom->loadAssets(checkpoint1::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void checkpoint1::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(checkpoint1::baseRMesh,startPos,destPos,posList);
}
