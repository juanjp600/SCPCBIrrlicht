#include "../room.h"
#include "room2servers.h"

RMesh* room2servers::baseRMesh = nullptr;

void room2servers::setBase(RMesh* inRme) {
	if (room2servers::baseRMesh==nullptr) {
		room2servers::baseRMesh = inRme;
	}
}

room2servers* room2servers::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2servers* retRoom = new room2servers;

	retRoom->loadAssets(room2servers::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2servers::getPointLights() {
	return room2servers::baseRMesh->pointlights;
}
