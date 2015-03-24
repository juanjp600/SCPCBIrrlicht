#include "../room.h"
#include "room079.h"

RMesh* room079::baseRMesh = nullptr;

void room079::setBase(RMesh* inRme) {
	if (room079::baseRMesh==nullptr) {
		room079::baseRMesh = inRme;
	}
}

room079* room079::createNew(irr::core::vector3df inPosition,char inAngle) {
	room079* retRoom = new room079;

	retRoom->loadAssets(room079::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room079::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room079::baseRMesh,startPos,destPos,posList);
}
