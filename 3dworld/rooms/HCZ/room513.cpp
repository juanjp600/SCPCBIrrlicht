#include "../room.h"
#include "room513.h"

RMesh* room513::baseRMesh = nullptr;

void room513::setBase(RMesh* inRme) {
	if (room513::baseRMesh==nullptr) {
		room513::baseRMesh = inRme;
	}
}

room513* room513::createNew(irr::core::vector3df inPosition,char inAngle) {
	room513* retRoom = new room513;

	retRoom->loadAssets(room513::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room513::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room513::baseRMesh,startPos,destPos,posList);
}
