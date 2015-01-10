#include "../room.h"
#include "room4.h"

RMesh* room4::baseRMesh = nullptr;

void room4::setBase(RMesh* inRme) {
	if (room4::baseRMesh==nullptr) {
		room4::baseRMesh = inRme;
	}
}

room4* room4::createNew(irr::core::vector3df inPosition,char inAngle) {
	room4* retRoom = new room4;

	retRoom->loadAssets(room4::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room4::getPointLights() {
	return room4::baseRMesh->pointlights;
}

void room4::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room4::baseRMesh,startPos,destPos,posList);
}
