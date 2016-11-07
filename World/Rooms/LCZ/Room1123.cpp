#include "../Room.h"
#include "Room1123.h"

RMesh* Room1123::baseRMesh = nullptr;

void Room1123::setBase(RMesh* inRme) {
	if (Room1123::baseRMesh==nullptr) {
		Room1123::baseRMesh = inRme;
	}
}

Room1123* Room1123::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room1123* retRoom = new Room1123;

	retRoom->loadAssets(Room1123::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room1123::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room1123::baseRMesh,startPos,destPos,posList);
}
