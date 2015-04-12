#include "../Room.h"
#include "Room3servers2.h"

RMesh* Room3servers2::baseRMesh = nullptr;

void Room3servers2::setBase(RMesh* inRme) {
	if (Room3servers2::baseRMesh==nullptr) {
		Room3servers2::baseRMesh = inRme;
	}
}

Room3servers2* Room3servers2::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3servers2* retRoom = new Room3servers2;

	retRoom->loadAssets(Room3servers2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3servers2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3servers2::baseRMesh,startPos,destPos,posList);
}
