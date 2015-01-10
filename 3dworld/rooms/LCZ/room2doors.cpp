#include "../room.h"
#include "room2doors.h"

RMesh* room2doors::baseRMesh = nullptr;

void room2doors::setBase(RMesh* inRme) {
	if (room2doors::baseRMesh==nullptr) {
		room2doors::baseRMesh = inRme;
	}
}

room2doors* room2doors::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2doors* retRoom = new room2doors;

	retRoom->loadAssets(room2doors::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2doors::getPointLights() {
	return room2doors::baseRMesh->pointlights;
}

void room2doors::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2doors::baseRMesh,startPos,destPos,posList);
}
