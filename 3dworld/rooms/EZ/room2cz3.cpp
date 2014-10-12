#include "../room.h"
#include "room2cz3.h"

RMesh* room2cz3::baseRMesh = nullptr;

void room2cz3::setBase(RMesh* inRme) {
	if (room2cz3::baseRMesh==nullptr) {
		room2cz3::baseRMesh = inRme;
	}
}

room2cz3* room2cz3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2cz3* retRoom = new room2cz3;

	retRoom->loadAssets(room2cz3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2cz3::getPointLights() {
	return room2cz3::baseRMesh->pointlights;
}
