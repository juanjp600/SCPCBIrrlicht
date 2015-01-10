#include "../room.h"
#include "room2tesla.h"

RMesh* room2tesla::baseRMesh = nullptr;

void room2tesla::setBase(RMesh* inRme) {
	if (room2tesla::baseRMesh==nullptr) {
		room2tesla::baseRMesh = inRme;
	}
}

room2tesla* room2tesla::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2tesla* retRoom = new room2tesla;

	retRoom->loadAssets(room2tesla::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2tesla::getPointLights() {
	return room2tesla::baseRMesh->pointlights;
}

void room2tesla::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2tesla::baseRMesh,startPos,destPos,posList);
}
