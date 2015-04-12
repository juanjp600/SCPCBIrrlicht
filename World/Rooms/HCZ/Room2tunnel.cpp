#include "../Room.h"
#include "Room2tunnel.h"

RMesh* Room2tunnel::baseRMesh = nullptr;

void Room2tunnel::setBase(RMesh* inRme) {
	if (Room2tunnel::baseRMesh==nullptr) {
		Room2tunnel::baseRMesh = inRme;
	}
}

Room2tunnel* Room2tunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2tunnel* retRoom = new Room2tunnel;

	retRoom->loadAssets(Room2tunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2tunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2tunnel::baseRMesh,startPos,destPos,posList);
}
