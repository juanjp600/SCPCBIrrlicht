#include "../room.h"
#include "room2poffices.h"

RMesh* room2poffices::baseRMesh = nullptr;

void room2poffices::setBase(RMesh* inRme) {
	if (room2poffices::baseRMesh==nullptr) {
		room2poffices::baseRMesh = inRme;
	}
}

room2poffices* room2poffices::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2poffices* retRoom = new room2poffices;

	retRoom->loadAssets(room2poffices::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2poffices::getPointLights() {
	return room2poffices::baseRMesh->pointlights;
}
