#include "../room.h"
#include "room2.h"

RMesh* room2::baseRMesh = nullptr;

void room2::setBase(RMesh* inRme) {
	if (room2::baseRMesh==nullptr) {
		room2::baseRMesh = inRme;
	}
}

room2* room2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2* retRoom = new room2;

	retRoom->loadAssets(room2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2::getPointLights() {
	return room2::baseRMesh->pointlights;
}

void room2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2::baseRMesh,startPos,destPos,posList);
}
