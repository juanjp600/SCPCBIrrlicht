#include "../room.h"
#include "room3.h"

RMesh* room3::baseRMesh = nullptr;

void room3::setBase(RMesh* inRme) {
	if (room3::baseRMesh==nullptr) {
		room3::baseRMesh = inRme;
	}
}

room3* room3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3* retRoom = new room3;

	retRoom->loadAssets(room3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3::getPointLights() {
	return room3::baseRMesh->pointlights;
}
