#include "../room.h"
#include "gatea.h"

RMesh* gatea::baseRMesh = nullptr;

void gatea::setBase(RMesh* inRme) {
	if (gatea::baseRMesh==nullptr) {
		gatea::baseRMesh = inRme;
	}
}

gatea* gatea::createNew(irr::core::vector3df inPosition,char inAngle) {
	gatea* retRoom = new gatea;

	retRoom->loadAssets(gatea::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void gatea::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(gatea::baseRMesh,startPos,destPos,posList);
}
