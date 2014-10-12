#include "../room.h"
#include "room2closets.h"

RMesh* room2closets::baseRMesh = nullptr;

void room2closets::setBase(RMesh* inRme) {
	if (room2closets::baseRMesh==nullptr) {
		room2closets::baseRMesh = inRme;
	}
}

room2closets* room2closets::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2closets* retRoom = new room2closets;

	retRoom->loadAssets(room2closets::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2closets::getPointLights() {
	return room2closets::baseRMesh->pointlights;
}
