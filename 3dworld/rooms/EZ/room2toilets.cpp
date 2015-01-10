#include "../room.h"
#include "room2toilets.h"

RMesh* room2toilets::baseRMesh = nullptr;

void room2toilets::setBase(RMesh* inRme) {
	if (room2toilets::baseRMesh==nullptr) {
		room2toilets::baseRMesh = inRme;
	}
}

room2toilets* room2toilets::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2toilets* retRoom = new room2toilets;

	retRoom->loadAssets(room2toilets::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2toilets::getPointLights() {
	return room2toilets::baseRMesh->pointlights;
}

void room2toilets::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2toilets::baseRMesh,startPos,destPos,posList);
}
