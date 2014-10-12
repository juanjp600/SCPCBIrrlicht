#include "../room.h"
#include "room3z3.h"

RMesh* room3z3::baseRMesh = nullptr;

void room3z3::setBase(RMesh* inRme) {
	if (room3z3::baseRMesh==nullptr) {
		room3z3::baseRMesh = inRme;
	}
}

room3z3* room3z3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3z3* retRoom = new room3z3;

	retRoom->loadAssets(room3z3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room3z3::getPointLights() {
	return room3z3::baseRMesh->pointlights;
}
