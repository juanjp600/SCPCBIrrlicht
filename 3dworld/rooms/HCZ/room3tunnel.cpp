#include "../room.h"
#include "room3tunnel.h"

RMesh* room3tunnel::baseRMesh = nullptr;

void room3tunnel::setBase(RMesh* inRme) {
	if (room3tunnel::baseRMesh==nullptr) {
		room3tunnel::baseRMesh = inRme;
	}
}

room3tunnel* room3tunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3tunnel* retRoom = new room3tunnel;

	retRoom->loadAssets(room3tunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3tunnel::getPointLights() {
	return room3tunnel::baseRMesh->pointlights;
}
