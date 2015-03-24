#include "../room.h"
#include "checkpoint2.h"

RMesh* checkpoint2::baseRMesh = nullptr;

void checkpoint2::setBase(RMesh* inRme) {
	if (checkpoint2::baseRMesh==nullptr) {
		checkpoint2::baseRMesh = inRme;
	}
}

checkpoint2* checkpoint2::createNew(irr::core::vector3df inPosition,char inAngle) {
	checkpoint2* retRoom = new checkpoint2;

	retRoom->loadAssets(checkpoint2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void checkpoint2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(checkpoint2::baseRMesh,startPos,destPos,posList);
}
