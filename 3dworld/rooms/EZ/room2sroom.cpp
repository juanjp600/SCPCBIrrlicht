#include "../room.h"
#include "room2sroom.h"

RMesh* room2sroom::baseRMesh = nullptr;

void room2sroom::setBase(RMesh* inRme) {
	if (room2sroom::baseRMesh==nullptr) {
		room2sroom::baseRMesh = inRme;
	}
}

room2sroom* room2sroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2sroom* retRoom = new room2sroom;

	retRoom->loadAssets(room2sroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2sroom::getPointLights() {
	return room2sroom::baseRMesh->pointlights;
}
