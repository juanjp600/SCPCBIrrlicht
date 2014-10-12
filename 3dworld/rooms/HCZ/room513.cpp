#include "../room.h"
#include "room513.h"

RMesh* room513::baseRMesh = nullptr;

void room513::setBase(RMesh* inRme) {
	if (room513::baseRMesh==nullptr) {
		room513::baseRMesh = inRme;
	}
}

room513* room513::createNew(irr::core::vector3df inPosition,char inAngle) {
	room513* retRoom = new room513;

	retRoom->loadAssets(room513::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room513::getPointLights() {
	return room513::baseRMesh->pointlights;
}
