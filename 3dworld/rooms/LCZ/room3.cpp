#include "../room.h"
#include "room3.h"

RMesh* room3::baseRMesh = nullptr;

void room3::setBase(RMesh* inRme) {
	if (room3::baseRMesh==nullptr) {
		room3::baseRMesh = inRme;
	}
}

room3* room3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3* retRoom = new room3;

	retRoom->loadAssets(room3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room3::baseRMesh,startPos,destPos,posList);
}
