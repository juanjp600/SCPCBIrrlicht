#include "../room.h"
#include "room2elevator.h"

RMesh* room2elevator::baseRMesh = nullptr;

void room2elevator::setBase(RMesh* inRme) {
	if (room2elevator::baseRMesh==nullptr) {
		room2elevator::baseRMesh = inRme;
	}
}

room2elevator* room2elevator::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2elevator* retRoom = new room2elevator;

	retRoom->loadAssets(room2elevator::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2elevator::getPointLights() {
	return room2elevator::baseRMesh->pointlights;
}
