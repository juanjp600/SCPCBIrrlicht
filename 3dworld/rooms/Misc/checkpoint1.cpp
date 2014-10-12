#include "../room.h"
#include "checkpoint1.h"

RMesh* checkpoint1::baseRMesh = nullptr;

void checkpoint1::setBase(RMesh* inRme) {
	if (checkpoint1::baseRMesh==nullptr) {
		checkpoint1::baseRMesh = inRme;
	}
}

checkpoint1* checkpoint1::createNew(irr::core::vector3df inPosition,char inAngle) {
	checkpoint1* retRoom = new checkpoint1;

	retRoom->loadAssets(checkpoint1::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& checkpoint1::getPointLights() {
	return checkpoint1::baseRMesh->pointlights;
}
