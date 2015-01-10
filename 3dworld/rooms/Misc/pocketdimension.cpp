#include "../room.h"
#include "pocketdimension.h"

RMesh* pocketdimension::baseRMesh = nullptr;

void pocketdimension::setBase(RMesh* inRme) {
	if (pocketdimension::baseRMesh==nullptr) {
		pocketdimension::baseRMesh = inRme;
	}
}

pocketdimension* pocketdimension::createNew(irr::core::vector3df inPosition,char inAngle) {
	pocketdimension* retRoom = new pocketdimension;

	retRoom->loadAssets(pocketdimension::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& pocketdimension::getPointLights() {
	return pocketdimension::baseRMesh->pointlights;
}

void pocketdimension::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(pocketdimension::baseRMesh,startPos,destPos,posList);
}
