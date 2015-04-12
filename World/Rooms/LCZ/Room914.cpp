#include "../Room.h"
#include "Room914.h"

RMesh* Room914::baseRMesh = nullptr;

void Room914::setBase(RMesh* inRme) {
	if (Room914::baseRMesh==nullptr) {
		Room914::baseRMesh = inRme;
	}
}

Room914* Room914::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room914* retRoom = new Room914;

	retRoom->loadAssets(Room914::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room914::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room914::baseRMesh,startPos,destPos,posList);
}
