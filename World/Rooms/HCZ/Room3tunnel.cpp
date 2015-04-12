#include "../Room.h"
#include "Room3tunnel.h"

RMesh* Room3tunnel::baseRMesh = nullptr;

void Room3tunnel::setBase(RMesh* inRme) {
	if (Room3tunnel::baseRMesh==nullptr) {
		Room3tunnel::baseRMesh = inRme;
	}
}

Room3tunnel* Room3tunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3tunnel* retRoom = new Room3tunnel;

	retRoom->loadAssets(Room3tunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3tunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3tunnel::baseRMesh,startPos,destPos,posList);
}
