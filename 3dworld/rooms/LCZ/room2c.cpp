#include "../room.h"
#include "room2c.h"

RMesh* room2c::baseRMesh = nullptr;

void room2c::setBase(RMesh* inRme) {
	if (room2c::baseRMesh==nullptr) {
		room2c::baseRMesh = inRme;
	}
}

room2c* room2c::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2c* retRoom = new room2c;

	retRoom->loadAssets(room2c::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2c::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2c::baseRMesh,startPos,destPos,posList);
}
