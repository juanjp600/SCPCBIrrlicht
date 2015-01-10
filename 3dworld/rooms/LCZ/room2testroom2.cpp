#include "../room.h"
#include "room2testroom2.h"

RMesh* room2testroom2::baseRMesh = nullptr;

void room2testroom2::setBase(RMesh* inRme) {
	if (room2testroom2::baseRMesh==nullptr) {
		room2testroom2::baseRMesh = inRme;
	}
}

room2testroom2* room2testroom2::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2testroom2* retRoom = new room2testroom2;

	retRoom->loadAssets(room2testroom2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2testroom2::getPointLights() {
	return room2testroom2::baseRMesh->pointlights;
}

void room2testroom2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2testroom2::baseRMesh,startPos,destPos,posList);
}
