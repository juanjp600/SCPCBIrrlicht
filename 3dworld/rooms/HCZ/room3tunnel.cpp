#include "../room.h"
#include "room3tunnel.h"

RMesh* room3tunnel::baseRMesh = nullptr;

void room3tunnel::setBase(RMesh* inRme) {
	if (room3tunnel::baseRMesh==nullptr) {
		room3tunnel::baseRMesh = inRme;
	}
}

room3tunnel* room3tunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3tunnel* retRoom = new room3tunnel;

	retRoom->loadAssets(room3tunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room3tunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room3tunnel::baseRMesh,startPos,destPos,posList);
}
