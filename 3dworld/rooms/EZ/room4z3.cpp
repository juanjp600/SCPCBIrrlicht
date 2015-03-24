#include "../room.h"
#include "room4z3.h"

RMesh* room4z3::baseRMesh = nullptr;

void room4z3::setBase(RMesh* inRme) {
	if (room4z3::baseRMesh==nullptr) {
		room4z3::baseRMesh = inRme;
	}
}

room4z3* room4z3::createNew(irr::core::vector3df inPosition,char inAngle) {
	room4z3* retRoom = new room4z3;

	retRoom->loadAssets(room4z3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room4z3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room4z3::baseRMesh,startPos,destPos,posList);
}
