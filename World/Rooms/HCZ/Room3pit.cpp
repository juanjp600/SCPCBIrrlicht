#include "../Room.h"
#include "Room3pit.h"

RMesh* Room3pit::baseRMesh = nullptr;

void Room3pit::setBase(RMesh* inRme) {
	if (Room3pit::baseRMesh==nullptr) {
		Room3pit::baseRMesh = inRme;
	}
}

Room3pit* Room3pit::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3pit* retRoom = new Room3pit;

	retRoom->loadAssets(Room3pit::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3pit::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3pit::baseRMesh,startPos,destPos,posList);
}
