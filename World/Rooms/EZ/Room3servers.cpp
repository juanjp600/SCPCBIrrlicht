#include "../Room.h"
#include "Room3servers.h"

RMesh* Room3servers::baseRMesh = nullptr;

void Room3servers::setBase(RMesh* inRme) {
	if (Room3servers::baseRMesh==nullptr) {
		Room3servers::baseRMesh = inRme;
	}
}

Room3servers* Room3servers::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3servers* retRoom = new Room3servers;

	retRoom->loadAssets(Room3servers::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3servers::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3servers::baseRMesh,startPos,destPos,posList);
}
