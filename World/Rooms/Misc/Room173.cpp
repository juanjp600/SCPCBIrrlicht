#include "../Room.h"
#include "Room173.h"

RMesh* Room173::baseRMesh = nullptr;

void Room173::setBase(RMesh* inRme) {
	if (Room173::baseRMesh==nullptr) {
		Room173::baseRMesh = inRme;
	}
}

Room173* Room173::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room173* retRoom = new Room173;

	retRoom->loadAssets(Room173::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room173::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room173::baseRMesh,startPos,destPos,posList);
}
