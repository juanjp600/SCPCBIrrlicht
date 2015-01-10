#include "../room.h"
#include "endroom.h"

RMesh* endroom::baseRMesh = nullptr;

void endroom::setBase(RMesh* inRme) {
	if (endroom::baseRMesh==nullptr) {
		endroom::baseRMesh = inRme;
	}
}

endroom* endroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	endroom* retRoom = new endroom;

	retRoom->loadAssets(endroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& endroom::getPointLights() {
	return endroom::baseRMesh->pointlights;
}

void endroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(endroom::baseRMesh,startPos,destPos,posList);
}
