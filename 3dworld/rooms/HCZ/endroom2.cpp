#include "../room.h"
#include "endroom2.h"

RMesh* endroom2::baseRMesh = nullptr;

void endroom2::setBase(RMesh* inRme) {
	if (endroom2::baseRMesh==nullptr) {
		endroom2::baseRMesh = inRme;
	}
}

endroom2* endroom2::createNew(irr::core::vector3df inPosition,char inAngle) {
	endroom2* retRoom = new endroom2;

	retRoom->loadAssets(endroom2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& endroom2::getPointLights() {
	return endroom2::baseRMesh->pointlights;
}

void endroom2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(endroom2::baseRMesh,startPos,destPos,posList);
}
