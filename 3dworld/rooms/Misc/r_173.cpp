#include "../room.h"
#include "r_173.h"

RMesh* r_173::baseRMesh = nullptr;

void r_173::setBase(RMesh* inRme) {
	if (r_173::baseRMesh==nullptr) {
		r_173::baseRMesh = inRme;
	}
}

r_173* r_173::createNew(irr::core::vector3df inPosition,char inAngle) {
	r_173* retRoom = new r_173;

	retRoom->loadAssets(r_173::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& r_173::getPointLights() {
	return r_173::baseRMesh->pointlights;
}
