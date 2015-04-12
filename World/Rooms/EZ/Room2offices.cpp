#include "../Room.h"
#include "Room2offices.h"

RMesh* Room2offices::baseRMesh = nullptr;

void Room2offices::setBase(RMesh* inRme) {
	if (Room2offices::baseRMesh==nullptr) {
		Room2offices::baseRMesh = inRme;
	}
}

Room2offices* Room2offices::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2offices* retRoom = new Room2offices;

	retRoom->loadAssets(Room2offices::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2offices::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2offices::baseRMesh,startPos,destPos,posList);
}
