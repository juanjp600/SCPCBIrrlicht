#include "../Room.h"
#include "Room2offices2.h"

RMesh* Room2offices2::baseRMesh = nullptr;

void Room2offices2::setBase(RMesh* inRme) {
	if (Room2offices2::baseRMesh==nullptr) {
		Room2offices2::baseRMesh = inRme;
	}
}

Room2offices2* Room2offices2::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2offices2* retRoom = new Room2offices2;

	retRoom->loadAssets(Room2offices2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2offices2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2offices2::baseRMesh,startPos,destPos,posList);
}
