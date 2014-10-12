#include "../room.h"
#include "start.h"

RMesh* start::baseRMesh = nullptr;

void start::setBase(RMesh* inRme) {
	if (start::baseRMesh==nullptr) {
		start::baseRMesh = inRme;
	}
}

start* start::createNew(irr::core::vector3df inPosition,char inAngle) {
	start* retRoom = new start;

	retRoom->loadAssets(start::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& start::getPointLights() {
	return start::baseRMesh->pointlights;
}
