#include "../room.h"
#include "room2pipes.h"

RMesh* room2pipes::baseRMesh = nullptr;

void room2pipes::setBase(RMesh* inRme) {
	if (room2pipes::baseRMesh==nullptr) {
		room2pipes::baseRMesh = inRme;
	}
}

room2pipes* room2pipes::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2pipes* retRoom = new room2pipes;

	retRoom->loadAssets(room2pipes::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2pipes::getPointLights() {
	return room2pipes::baseRMesh->pointlights;
}

void room2pipes::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2pipes::baseRMesh,startPos,destPos,posList);
}
