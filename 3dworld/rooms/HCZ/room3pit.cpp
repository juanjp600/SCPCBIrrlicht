#include "../room.h"
#include "room3pit.h"

RMesh* room3pit::baseRMesh = nullptr;

void room3pit::setBase(RMesh* inRme) {
	if (room3pit::baseRMesh==nullptr) {
		room3pit::baseRMesh = inRme;
	}
}

room3pit* room3pit::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3pit* retRoom = new room3pit;

	retRoom->loadAssets(room3pit::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room3pit::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room3pit::baseRMesh,startPos,destPos,posList);
}
