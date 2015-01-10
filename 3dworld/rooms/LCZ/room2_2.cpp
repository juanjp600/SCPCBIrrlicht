#include "../room.h"
#include "room2_2.h"

RMesh* room2_2::baseRMesh = nullptr;

void room2_2::setBase(RMesh* inRme) {
	if (room2_2::baseRMesh==nullptr) {
		room2_2::baseRMesh = inRme;
	}
}

room2_2* room2_2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2_2* retRoom = new room2_2;

	retRoom->loadAssets(room2_2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2_2::getPointLights() {
	return room2_2::baseRMesh->pointlights;
}

void room2_2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2_2::baseRMesh,startPos,destPos,posList);
}
