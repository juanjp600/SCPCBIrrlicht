#include "../room.h"
#include "room2scps.h"

RMesh* room2scps::baseRMesh = nullptr;

void room2scps::setBase(RMesh* inRme) {
	if (room2scps::baseRMesh==nullptr) {
		room2scps::baseRMesh = inRme;
	}
}

room2scps* room2scps::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2scps* retRoom = new room2scps;

	retRoom->loadAssets(room2scps::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2scps::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2scps::baseRMesh,startPos,destPos,posList);
}
