#include "../room.h"
#include "coffin.h"

RMesh* coffin::baseRMesh = nullptr;

void coffin::setBase(RMesh* inRme) {
	if (coffin::baseRMesh==nullptr) {
		coffin::baseRMesh = inRme;
	}
}

coffin* coffin::createNew(irr::core::vector3df inPosition,char inAngle) {
	coffin* retRoom = new coffin;

	retRoom->loadAssets(coffin::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void coffin::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(coffin::baseRMesh,startPos,destPos,posList);
}
