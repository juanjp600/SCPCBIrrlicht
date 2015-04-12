#include "../Room.h"
#include "Room012.h"

RMesh* Room012::baseRMesh = nullptr;

void Room012::setBase(RMesh* inRme) {
	if (Room012::baseRMesh==nullptr) {
		Room012::baseRMesh = inRme;
	}
}

Room012* Room012::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room012* retRoom = new Room012;

	retRoom->loadAssets(Room012::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room012::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room012::baseRMesh,startPos,destPos,posList);
}
