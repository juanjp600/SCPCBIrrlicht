#include "../room.h"
#include "room2storage.h"

RMesh* room2storage::baseRMesh = nullptr;

void room2storage::setBase(RMesh* inRme) {
	if (room2storage::baseRMesh==nullptr) {
		room2storage::baseRMesh = inRme;
	}
}

room2storage* room2storage::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2storage* retRoom = new room2storage;

	retRoom->loadAssets(room2storage::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2storage::getPointLights() {
	return room2storage::baseRMesh->pointlights;
}
