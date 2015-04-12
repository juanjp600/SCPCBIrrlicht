#include "../Room.h"
#include "Room3_2.h"

RMesh* Room3_2::baseRMesh = nullptr;

void Room3_2::setBase(RMesh* inRme) {
	if (Room3_2::baseRMesh==nullptr) {
		Room3_2::baseRMesh = inRme;
	}
}

Room3_2* Room3_2::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3_2* retRoom = new Room3_2;

	retRoom->loadAssets(Room3_2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3_2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3_2::baseRMesh,startPos,destPos,posList);
}
