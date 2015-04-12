#include "../Room.h"
#include "Room2c.h"

RMesh* Room2c::baseRMesh = nullptr;

void Room2c::setBase(RMesh* inRme) {
	if (Room2c::baseRMesh==nullptr) {
		Room2c::baseRMesh = inRme;
	}
}

Room2c* Room2c::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2c* retRoom = new Room2c;

	retRoom->loadAssets(Room2c::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2c::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2c::baseRMesh,startPos,destPos,posList);
}
