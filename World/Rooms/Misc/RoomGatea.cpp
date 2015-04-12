#include "../Room.h"
#include "RoomGatea.h"

RMesh* RoomGatea::baseRMesh = nullptr;

void RoomGatea::setBase(RMesh* inRme) {
	if (RoomGatea::baseRMesh==nullptr) {
		RoomGatea::baseRMesh = inRme;
	}
}

RoomGatea* RoomGatea::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomGatea* retRoom = new RoomGatea;

	retRoom->loadAssets(RoomGatea::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomGatea::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomGatea::baseRMesh,startPos,destPos,posList);
}
