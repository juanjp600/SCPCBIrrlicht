#include "../room.h"
#include "room2offices2.h"

RMesh* room2offices2::baseRMesh = nullptr;

void room2offices2::setBase(RMesh* inRme) {
	if (room2offices2::baseRMesh==nullptr) {
		room2offices2::baseRMesh = inRme;
	}
}

room2offices2* room2offices2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2offices2* retRoom = new room2offices2;

	retRoom->loadAssets(room2offices2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2offices2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2offices2::baseRMesh,startPos,destPos,posList);
}
