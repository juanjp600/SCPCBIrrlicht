#include "../room.h"
#include "room2ccont.h"

RMesh* room2ccont::baseRMesh = nullptr;

void room2ccont::setBase(RMesh* inRme) {
	if (room2ccont::baseRMesh==nullptr) {
		room2ccont::baseRMesh = inRme;
	}
}

room2ccont* room2ccont::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2ccont* retRoom = new room2ccont;

	retRoom->loadAssets(room2ccont::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& room2ccont::getPointLights() {
	return room2ccont::baseRMesh->pointlights;
}

void room2ccont::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2ccont::baseRMesh,startPos,destPos,posList);
}
