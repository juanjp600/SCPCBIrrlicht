#include "../room.h"
#include "room035.h"

RMesh* room035::baseRMesh = nullptr;

void room035::setBase(RMesh* inRme) {
	if (room035::baseRMesh==nullptr) {
		room035::baseRMesh = inRme;
	}
}

room035* room035::createNew(irr::core::vector3df inPosition,char inAngle) {
	room035* retRoom = new room035;

	retRoom->loadAssets(room035::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room035::getPointLights() {
	return room035::baseRMesh->pointlights;
}

void room035::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room035::baseRMesh,startPos,destPos,posList);
}
