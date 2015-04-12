#include "../Room.h"
#include "Room035.h"

RMesh* Room035::baseRMesh = nullptr;

void Room035::setBase(RMesh* inRme) {
	if (Room035::baseRMesh==nullptr) {
		Room035::baseRMesh = inRme;
	}
}

Room035* Room035::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room035* retRoom = new Room035;

	retRoom->loadAssets(Room035::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room035::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room035::baseRMesh,startPos,destPos,posList);
}
