#include "../room.h"
#include "room860.h"

RMesh* room860::baseRMesh = nullptr;

void room860::setBase(RMesh* inRme) {
	if (room860::baseRMesh==nullptr) {
		room860::baseRMesh = inRme;
	}
}

room860* room860::createNew(irr::core::vector3df inPosition,char inAngle) {
	room860* retRoom = new room860;

	retRoom->loadAssets(room860::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room860::getPointLights() {
	return room860::baseRMesh->pointlights;
}

void room860::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room860::baseRMesh,startPos,destPos,posList);
}
