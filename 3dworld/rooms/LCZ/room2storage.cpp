#include "../room.h"
#include "room2storage.h"

RMesh* room2storage::baseRMesh = nullptr;

void room2storage::setBase(RMesh* inRme) {
	if (room2storage::baseRMesh==nullptr) {
		room2storage::baseRMesh = inRme;
	}
}

room2storage* room2storage::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2storage* retRoom = new room2storage;

	retRoom->loadAssets(room2storage::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2storage::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2storage::baseRMesh,startPos,destPos,posList);
}
