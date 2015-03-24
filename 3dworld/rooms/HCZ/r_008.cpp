#include "../room.h"
#include "r_008.h"

RMesh* r_008::baseRMesh = nullptr;

void r_008::setBase(RMesh* inRme) {
	if (r_008::baseRMesh==nullptr) {
		r_008::baseRMesh = inRme;
	}
}

r_008* r_008::createNew(irr::core::vector3df inPosition,char inAngle) {
	r_008* retRoom = new r_008;

	retRoom->loadAssets(r_008::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void r_008::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(r_008::baseRMesh,startPos,destPos,posList);
}
