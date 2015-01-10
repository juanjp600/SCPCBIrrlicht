#include "../room.h"
#include "room2z3.h"

RMesh* room2z3::baseRMesh = nullptr;

void room2z3::setBase(RMesh* inRme) {
	if (room2z3::baseRMesh==nullptr) {
		room2z3::baseRMesh = inRme;
	}
}

room2z3* room2z3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2z3* retRoom = new room2z3;

	retRoom->loadAssets(room2z3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2z3::getPointLights() {
	return room2z3::baseRMesh->pointlights;
}

void room2z3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2z3::baseRMesh,startPos,destPos,posList);
}
