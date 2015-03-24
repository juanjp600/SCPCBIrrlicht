#include "../room.h"
#include "room2pit.h"

RMesh* room2pit::baseRMesh = nullptr;

void room2pit::setBase(RMesh* inRme) {
	if (room2pit::baseRMesh==nullptr) {
		room2pit::baseRMesh = inRme;
	}
}

room2pit* room2pit::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2pit* retRoom = new room2pit;

	retRoom->loadAssets(room2pit::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2pit::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2pit::baseRMesh,startPos,destPos,posList);
}
