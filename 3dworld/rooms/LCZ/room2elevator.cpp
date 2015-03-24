#include "../room.h"
#include "room2elevator.h"

RMesh* room2elevator::baseRMesh = nullptr;

void room2elevator::setBase(RMesh* inRme) {
	if (room2elevator::baseRMesh==nullptr) {
		room2elevator::baseRMesh = inRme;
	}
}

room2elevator* room2elevator::createNew(irr::core::vector3df inPosition,char inAngle) {
	room2elevator* retRoom = new room2elevator;

	retRoom->loadAssets(room2elevator::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room2elevator::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room2elevator::baseRMesh,startPos,destPos,posList);
}
