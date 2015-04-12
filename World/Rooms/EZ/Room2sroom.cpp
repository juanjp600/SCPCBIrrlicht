#include "../Room.h"
#include "Room2sroom.h"

RMesh* Room2sroom::baseRMesh = nullptr;

void Room2sroom::setBase(RMesh* inRme) {
	if (Room2sroom::baseRMesh==nullptr) {
		Room2sroom::baseRMesh = inRme;
	}
}

Room2sroom* Room2sroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2sroom* retRoom = new Room2sroom;

	retRoom->loadAssets(Room2sroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2sroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2sroom::baseRMesh,startPos,destPos,posList);
}
