#include "../room.h"
#include "room2cafeteria.h"

RMesh* room2cafeteria::baseRMesh = nullptr;

void room2cafeteria::setBase(RMesh* inRme) {
	if (room2cafeteria::baseRMesh==nullptr) {
		room2cafeteria::baseRMesh = inRme;
	}
}

room2cafeteria* room2cafeteria::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2cafeteria* retRoom = new room2cafeteria;

	retRoom->loadAssets(room2cafeteria::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2cafeteria::getPointLights() {
	return room2cafeteria::baseRMesh->pointlights;
}
