#include "../room.h"
#include "room3servers.h"

RMesh* room3servers::baseRMesh = nullptr;

void room3servers::setBase(RMesh* inRme) {
	if (room3servers::baseRMesh==nullptr) {
		room3servers::baseRMesh = inRme;
	}
}

room3servers* room3servers::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3servers* retRoom = new room3servers;

	retRoom->loadAssets(room3servers::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3servers::getPointLights() {
	return room3servers::baseRMesh->pointlights;
}
