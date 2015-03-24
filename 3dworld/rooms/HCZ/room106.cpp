#include "../room.h"
#include "room106.h"

RMesh* room106::baseRMesh = nullptr;

void room106::setBase(RMesh* inRme) {
	if (room106::baseRMesh==nullptr) {
		room106::baseRMesh = inRme;
	}
}

room106* room106::createNew(irr::core::vector3df inPosition,char inAngle) {
	room106* retRoom = new room106;

	retRoom->loadAssets(room106::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room106::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room106::baseRMesh,startPos,destPos,posList);
}
