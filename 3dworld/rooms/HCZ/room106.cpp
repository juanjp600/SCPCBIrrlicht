#include "../room.h"
#include "room106.h"

RMesh* room106::baseRMesh = nullptr;

void room106::setBase(RMesh* inRme) {
	if (room106::baseRMesh==nullptr) {
		room106::baseRMesh = inRme;
	}
}

room106* room106::createNew(irr::core::vector3df inPosition,char inAngle) {
	room106* retRoom = new room106;

	retRoom->loadAssets(room106::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room106::getPointLights() {
	return room106::baseRMesh->pointlights;
}
