#include "../Room.h"
#include "Room079.h"

RMesh* Room079::baseRMesh = nullptr;

void Room079::setBase(RMesh* inRme) {
	if (Room079::baseRMesh==nullptr) {
		Room079::baseRMesh = inRme;
	}
}

Room079* Room079::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room079* retRoom = new Room079;

	retRoom->loadAssets(Room079::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room079::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room079::baseRMesh,startPos,destPos,posList);
}
