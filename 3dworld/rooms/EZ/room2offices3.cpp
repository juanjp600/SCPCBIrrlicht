#include "../room.h"
#include "room2offices3.h"

RMesh* room2offices3::baseRMesh = nullptr;

void room2offices3::setBase(RMesh* inRme) {
	if (room2offices3::baseRMesh==nullptr) {
		room2offices3::baseRMesh = inRme;
	}
}

room2offices3* room2offices3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2offices3* retRoom = new room2offices3;

	retRoom->loadAssets(room2offices3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2offices3::getPointLights() {
	return room2offices3::baseRMesh->pointlights;
}
