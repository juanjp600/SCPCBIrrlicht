#include "../room.h"
#include "room2cz3.h"

RMesh* room2cz3::baseRMesh = nullptr;

void room2cz3::setBase(RMesh* inRme) {
	if (room2cz3::baseRMesh==nullptr) {
		room2cz3::baseRMesh = inRme;
	}
}

room2cz3* room2cz3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2cz3* retRoom = new room2cz3;

	retRoom->loadAssets(room2cz3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2cz3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2cz3::baseRMesh,startPos,destPos,posList);
}
