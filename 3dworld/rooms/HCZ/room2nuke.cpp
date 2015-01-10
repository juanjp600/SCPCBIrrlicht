#include "../room.h"
#include "room2nuke.h"

RMesh* room2nuke::baseRMesh = nullptr;

void room2nuke::setBase(RMesh* inRme) {
	if (room2nuke::baseRMesh==nullptr) {
		room2nuke::baseRMesh = inRme;
	}
}

room2nuke* room2nuke::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2nuke* retRoom = new room2nuke;

	retRoom->loadAssets(room2nuke::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2nuke::getPointLights() {
	return room2nuke::baseRMesh->pointlights;
}

void room2nuke::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2nuke::baseRMesh,startPos,destPos,posList);
}
