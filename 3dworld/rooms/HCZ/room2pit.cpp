#include "../room.h"
#include "room2pit.h"

RMesh* room2pit::baseRMesh = nullptr;

void room2pit::setBase(RMesh* inRme) {
	if (room2pit::baseRMesh==nullptr) {
		room2pit::baseRMesh = inRme;
	}
}

room2pit* room2pit::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2pit* retRoom = new room2pit;

	retRoom->loadAssets(room2pit::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2pit::getPointLights() {
	return room2pit::baseRMesh->pointlights;
}
