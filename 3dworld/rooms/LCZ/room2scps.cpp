#include "../room.h"
#include "room2scps.h"

RMesh* room2scps::baseRMesh = nullptr;

void room2scps::setBase(RMesh* inRme) {
	if (room2scps::baseRMesh==nullptr) {
		room2scps::baseRMesh = inRme;
	}
}

room2scps* room2scps::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2scps* retRoom = new room2scps;

	retRoom->loadAssets(room2scps::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2scps::getPointLights() {
	return room2scps::baseRMesh->pointlights;
}
