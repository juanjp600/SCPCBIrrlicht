#include "../room.h"
#include "room3servers2.h"

RMesh* room3servers2::baseRMesh = nullptr;

void room3servers2::setBase(RMesh* inRme) {
	if (room3servers2::baseRMesh==nullptr) {
		room3servers2::baseRMesh = inRme;
	}
}

room3servers2* room3servers2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3servers2* retRoom = new room3servers2;

	retRoom->loadAssets(room3servers2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3servers2::getPointLights() {
	return room3servers2::baseRMesh->pointlights;
}
