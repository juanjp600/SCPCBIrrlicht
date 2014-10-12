#include "../room.h"
#include "room079.h"

RMesh* room079::baseRMesh = nullptr;

void room079::setBase(RMesh* inRme) {
	if (room079::baseRMesh==nullptr) {
		room079::baseRMesh = inRme;
	}
}

room079* room079::createNew(irr::core::vector3df inPosition,char inAngle) {
	room079* retRoom = new room079;

	retRoom->loadAssets(room079::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room079::getPointLights() {
	return room079::baseRMesh->pointlights;
}
