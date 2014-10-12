#include "../room.h"
#include "room2poffices2.h"

RMesh* room2poffices2::baseRMesh = nullptr;

void room2poffices2::setBase(RMesh* inRme) {
	if (room2poffices2::baseRMesh==nullptr) {
		room2poffices2::baseRMesh = inRme;
	}
}

room2poffices2* room2poffices2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2poffices2* retRoom = new room2poffices2;

	retRoom->loadAssets(room2poffices2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2poffices2::getPointLights() {
	return room2poffices2::baseRMesh->pointlights;
}
