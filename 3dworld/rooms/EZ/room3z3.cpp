#include "../room.h"
#include "room3z3.h"

RMesh* room3z3::baseRMesh = nullptr;

void room3z3::setBase(RMesh* inRme) {
	if (room3z3::baseRMesh==nullptr) {
		room3z3::baseRMesh = inRme;
	}
}

room3z3* room3z3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room3z3* retRoom = new room3z3;

	retRoom->loadAssets(room3z3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room3z3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room3z3::baseRMesh,startPos,destPos,posList);
}
