#include "../Room.h"
#include "Room2pit.h"

RMesh* Room2pit::baseRMesh = nullptr;

void Room2pit::setBase(RMesh* inRme) {
	if (Room2pit::baseRMesh==nullptr) {
		Room2pit::baseRMesh = inRme;
	}
}

Room2pit* Room2pit::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2pit* retRoom = new Room2pit;

	retRoom->loadAssets(Room2pit::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2pit::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2pit::baseRMesh,startPos,destPos,posList);
}
