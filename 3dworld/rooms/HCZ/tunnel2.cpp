#include "../room.h"
#include "tunnel2.h"

RMesh* tunnel2::baseRMesh = nullptr;

void tunnel2::setBase(RMesh* inRme) {
	if (tunnel2::baseRMesh==nullptr) {
		tunnel2::baseRMesh = inRme;
	}
}

tunnel2* tunnel2::createNew(irr::core::vector3df inPosition,char inAngle) {
	tunnel2* retRoom = new tunnel2;

	retRoom->loadAssets(tunnel2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& tunnel2::getPointLights() {
	return tunnel2::baseRMesh->pointlights;
}

void tunnel2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(tunnel2::baseRMesh,startPos,destPos,posList);
}
