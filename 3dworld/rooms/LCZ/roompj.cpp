#include "../room.h"
#include "roompj.h"

RMesh* roompj::baseRMesh = nullptr;

void roompj::setBase(RMesh* inRme) {
	if (roompj::baseRMesh==nullptr) {
		roompj::baseRMesh = inRme;
	}
}

roompj* roompj::createNew(irr::core::vector3df inPosition,char inAngle) {
	roompj* retRoom = new roompj;

	retRoom->loadAssets(roompj::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void roompj::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(roompj::baseRMesh,startPos,destPos,posList);
}
