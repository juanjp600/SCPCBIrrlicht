#include "../room.h"
#include "room2c.h"

RMesh* room2c::baseRMesh = nullptr;

void room2c::setBase(RMesh* inRme) {
	if (room2c::baseRMesh==nullptr) {
		room2c::baseRMesh = inRme;
	}
}

room2c* room2c::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2c* retRoom = new room2c;

	retRoom->loadAssets(room2c::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2c::getPointLights() {
	return room2c::baseRMesh->pointlights;
}
