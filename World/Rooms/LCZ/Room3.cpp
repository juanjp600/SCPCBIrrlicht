#include "../Room.h"
#include "Room3.h"

RMesh* Room3::baseRMesh = nullptr;

void Room3::setBase(RMesh* inRme) {
	if (Room3::baseRMesh==nullptr) {
		Room3::baseRMesh = inRme;
	}
}

Room3* Room3::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3* retRoom = new Room3;

	retRoom->loadAssets(Room3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3::baseRMesh,startPos,destPos,posList);
}
