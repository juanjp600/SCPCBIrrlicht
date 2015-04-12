#include "../Room.h"
#include "Room513.h"

RMesh* Room513::baseRMesh = nullptr;

void Room513::setBase(RMesh* inRme) {
	if (Room513::baseRMesh==nullptr) {
		Room513::baseRMesh = inRme;
	}
}

Room513* Room513::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room513* retRoom = new Room513;

	retRoom->loadAssets(Room513::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room513::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room513::baseRMesh,startPos,destPos,posList);
}
