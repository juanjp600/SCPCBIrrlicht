#include "../room.h"
#include "room3pit.h"

RMesh* room3pit::baseRMesh = nullptr;

void room3pit::setBase(RMesh* inRme) {
	if (room3pit::baseRMesh==nullptr) {
		room3pit::baseRMesh = inRme;
	}
}

room3pit* room3pit::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3pit* retRoom = new room3pit;

	retRoom->loadAssets(room3pit::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3pit::getPointLights() {
	return room3pit::baseRMesh->pointlights;
}
