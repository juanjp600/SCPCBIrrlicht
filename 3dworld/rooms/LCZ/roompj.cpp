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

const std::vector<irr::video::SLight>& roompj::getPointLights() {
	return roompj::baseRMesh->pointlights;
}
