#include "../room.h"
#include "room3storage.h"

RMesh* room3storage::baseRMesh = nullptr;

void room3storage::setBase(RMesh* inRme) {
	if (room3storage::baseRMesh==nullptr) {
		room3storage::baseRMesh = inRme;
	}
}

room3storage* room3storage::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3storage* retRoom = new room3storage;

	retRoom->loadAssets(room3storage::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3storage::getPointLights() {
	return room3storage::baseRMesh->pointlights;
}
