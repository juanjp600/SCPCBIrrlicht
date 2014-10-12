#include "../room.h"
#include "room3_2.h"

RMesh* room3_2::baseRMesh = nullptr;

void room3_2::setBase(RMesh* inRme) {
	if (room3_2::baseRMesh==nullptr) {
		room3_2::baseRMesh = inRme;
	}
}

room3_2* room3_2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3_2* retRoom = new room3_2;

	retRoom->loadAssets(room3_2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3_2::getPointLights() {
	return room3_2::baseRMesh->pointlights;
}
