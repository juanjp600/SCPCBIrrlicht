#include "../room.h"
#include "room012.h"

RMesh* room012::baseRMesh = nullptr;

void room012::setBase(RMesh* inRme) {
	if (room012::baseRMesh==nullptr) {
		room012::baseRMesh = inRme;
	}
}

room012* room012::createNew(irr::core::vector3df inPosition,char inAngle) {
	room012* retRoom = new room012;

	retRoom->loadAssets(room012::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room012::getPointLights() {
	return room012::baseRMesh->pointlights;
}

void room012::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room012::baseRMesh,startPos,destPos,posList);
}
