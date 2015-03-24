#include "../room.h"
#include "exit1.h"

RMesh* exit1::baseRMesh = nullptr;

void exit1::setBase(RMesh* inRme) {
	if (exit1::baseRMesh==nullptr) {
		exit1::baseRMesh = inRme;
	}
}

exit1* exit1::createNew(irr::core::vector3df inPosition,char inAngle) {
	exit1* retRoom = new exit1;

	retRoom->loadAssets(exit1::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void exit1::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(exit1::baseRMesh,startPos,destPos,posList);
}
