#include "../room.h"
#include "gateaentrance.h"

RMesh* gateaentrance::baseRMesh = nullptr;

void gateaentrance::setBase(RMesh* inRme) {
	if (gateaentrance::baseRMesh==nullptr) {
		gateaentrance::baseRMesh = inRme;
	}
}

gateaentrance* gateaentrance::createNew(irr::core::vector3df inPosition,char inAngle) {
	gateaentrance* retRoom = new gateaentrance;

	retRoom->loadAssets(gateaentrance::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& gateaentrance::getPointLights() {
	return gateaentrance::baseRMesh->pointlights;
}

void gateaentrance::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(gateaentrance::baseRMesh,startPos,destPos,posList);
}
