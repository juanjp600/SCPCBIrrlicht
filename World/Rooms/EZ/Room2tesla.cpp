#include "../Room.h"
#include "Room2tesla.h"

RMesh* Room2tesla::baseRMesh = nullptr;

void Room2tesla::setBase(RMesh* inRme) {
	if (Room2tesla::baseRMesh==nullptr) {
		Room2tesla::baseRMesh = inRme;
	}
}

Room2tesla* Room2tesla::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2tesla* retRoom = new Room2tesla;

	retRoom->loadAssets(Room2tesla::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2tesla::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2tesla::baseRMesh,startPos,destPos,posList);
}
