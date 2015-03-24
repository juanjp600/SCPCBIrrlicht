#include "../room.h"
#include "room2offices3.h"

RMesh* room2offices3::baseRMesh = nullptr;

void room2offices3::setBase(RMesh* inRme) {
	if (room2offices3::baseRMesh==nullptr) {
		room2offices3::baseRMesh = inRme;
	}
}

room2offices3* room2offices3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2offices3* retRoom = new room2offices3;

	retRoom->loadAssets(room2offices3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2offices3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2offices3::baseRMesh,startPos,destPos,posList);
}
