#include "../room.h"
#include "r_914.h"

RMesh* r_914::baseRMesh = nullptr;

void r_914::setBase(RMesh* inRme) {
	if (r_914::baseRMesh==nullptr) {
		r_914::baseRMesh = inRme;
	}
}

r_914* r_914::createNew(irr::core::vector3df inPosition,char inAngle) {
	r_914* retRoom = new r_914;

	retRoom->loadAssets(r_914::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& r_914::getPointLights() {
	return r_914::baseRMesh->pointlights;
}
