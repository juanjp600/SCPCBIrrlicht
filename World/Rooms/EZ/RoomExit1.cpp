#include "../Room.h"
#include "RoomExit1.h"

RMesh* RoomExit1::baseRMesh = nullptr;

void RoomExit1::setBase(RMesh* inRme) {
	if (RoomExit1::baseRMesh==nullptr) {
		RoomExit1::baseRMesh = inRme;
	}
}

RoomExit1* RoomExit1::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomExit1* retRoom = new RoomExit1;

	retRoom->loadAssets(RoomExit1::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomExit1::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomExit1::baseRMesh,startPos,destPos,posList);
}
