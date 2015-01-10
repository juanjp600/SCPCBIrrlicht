#include "../room.h"
#include "room2tunnel.h"

RMesh* room2tunnel::baseRMesh = nullptr;

void room2tunnel::setBase(RMesh* inRme) {
	if (room2tunnel::baseRMesh==nullptr) {
		room2tunnel::baseRMesh = inRme;
	}
}

room2tunnel* room2tunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2tunnel* retRoom = new room2tunnel;

	retRoom->loadAssets(room2tunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2tunnel::getPointLights() {
	return room2tunnel::baseRMesh->pointlights;
}

void room2tunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2tunnel::baseRMesh,startPos,destPos,posList);
}
