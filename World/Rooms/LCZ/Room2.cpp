#include "../Room.h"
#include "Room2.h"

RMesh* Room2::baseRMesh = nullptr;

void Room2::setBase(RMesh* inRme) {
	if (Room2::baseRMesh==nullptr) {
		Room2::baseRMesh = inRme;
	}
}

Room2* Room2::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2* retRoom = new Room2;

	retRoom->loadAssets(Room2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2::baseRMesh,startPos,destPos,posList);
}
