#include "../Room.h"
#include "Room2doors.h"

RMesh* Room2doors::baseRMesh = nullptr;

void Room2doors::setBase(RMesh* inRme) {
	if (Room2doors::baseRMesh==nullptr) {
		Room2doors::baseRMesh = inRme;
	}
}

Room2doors* Room2doors::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2doors* retRoom = new Room2doors;

	retRoom->loadAssets(Room2doors::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2doors::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2doors::baseRMesh,startPos,destPos,posList);
}
