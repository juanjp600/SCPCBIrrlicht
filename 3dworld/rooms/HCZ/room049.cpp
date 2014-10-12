#include "../room.h"
#include "room049.h"

RMesh* room049::baseRMesh = nullptr;

void room049::setBase(RMesh* inRme) {
	if (room049::baseRMesh==nullptr) {
		room049::baseRMesh = inRme;
	}
}

room049* room049::createNew(irr::core::vector3df inPosition,char inAngle) {
	room049* retRoom = new room049;

	retRoom->loadAssets(room049::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room049::getPointLights() {
	return room049::baseRMesh->pointlights;
}
