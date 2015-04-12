#include "../Room.h"
#include "Room049.h"

RMesh* Room049::baseRMesh = nullptr;

void Room049::setBase(RMesh* inRme) {
	if (Room049::baseRMesh==nullptr) {
		Room049::baseRMesh = inRme;
	}
}

Room049* Room049::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room049* retRoom = new Room049;

	retRoom->loadAssets(Room049::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room049::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room049::baseRMesh,startPos,destPos,posList);
}
