#include "../room.h"
#include "room2offices.h"

RMesh* room2offices::baseRMesh = nullptr;

void room2offices::setBase(RMesh* inRme) {
	if (room2offices::baseRMesh==nullptr) {
		room2offices::baseRMesh = inRme;
	}
}

room2offices* room2offices::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2offices* retRoom = new room2offices;

	retRoom->loadAssets(room2offices::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2offices::getPointLights() {
	return room2offices::baseRMesh->pointlights;
}
