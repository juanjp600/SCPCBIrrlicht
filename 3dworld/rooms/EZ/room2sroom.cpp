#include "../room.h"
#include "room2sroom.h"

RMesh* room2sroom::baseRMesh = nullptr;

void room2sroom::setBase(RMesh* inRme) {
	if (room2sroom::baseRMesh==nullptr) {
		room2sroom::baseRMesh = inRme;
	}
}

room2sroom* room2sroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2sroom* retRoom = new room2sroom;

	retRoom->loadAssets(room2sroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2sroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2sroom::baseRMesh,startPos,destPos,posList);
}
