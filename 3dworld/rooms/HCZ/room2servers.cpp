#include "../room.h"
#include "room2servers.h"

RMesh* room2servers::baseRMesh = nullptr;

void room2servers::setBase(RMesh* inRme) {
	if (room2servers::baseRMesh==nullptr) {
		room2servers::baseRMesh = inRme;
	}
}

room2servers* room2servers::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2servers* retRoom = new room2servers;

	retRoom->loadAssets(room2servers::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2servers::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2servers::baseRMesh,startPos,destPos,posList);
}
