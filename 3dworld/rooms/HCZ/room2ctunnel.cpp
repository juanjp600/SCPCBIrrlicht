#include "../room.h"
#include "room2ctunnel.h"

RMesh* room2ctunnel::baseRMesh = nullptr;

void room2ctunnel::setBase(RMesh* inRme) {
	if (room2ctunnel::baseRMesh==nullptr) {
		room2ctunnel::baseRMesh = inRme;
	}
}

room2ctunnel* room2ctunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2ctunnel* retRoom = new room2ctunnel;

	retRoom->loadAssets(room2ctunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2ctunnel::getPointLights() {
	return room2ctunnel::baseRMesh->pointlights;
}

void room2ctunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2ctunnel::baseRMesh,startPos,destPos,posList);
}
