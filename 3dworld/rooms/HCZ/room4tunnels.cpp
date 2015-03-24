#include "../room.h"
#include "room4tunnels.h"

RMesh* room4tunnels::baseRMesh = nullptr;

void room4tunnels::setBase(RMesh* inRme) {
	if (room4tunnels::baseRMesh==nullptr) {
		room4tunnels::baseRMesh = inRme;
	}
}

room4tunnels* room4tunnels::createNew(irr::core::vector3df inPosition,char inAngle) {
	room4tunnels* retRoom = new room4tunnels;

	retRoom->loadAssets(room4tunnels::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void room4tunnels::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(room4tunnels::baseRMesh,startPos,destPos,posList);
}
