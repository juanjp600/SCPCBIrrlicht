#include "../Room.h"
#include "Room205.h"

RMesh* Room205::baseRMesh = nullptr;

void Room205::setBase(RMesh* inRme) {
	if (Room205::baseRMesh==nullptr) {
		Room205::baseRMesh = inRme;
	}
}

Room205* Room205::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room205* retRoom = new Room205;

	retRoom->loadAssets(Room205::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room205::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room205::baseRMesh,startPos,destPos,posList);
}
