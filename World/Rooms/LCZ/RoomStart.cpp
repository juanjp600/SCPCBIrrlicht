#include "../Room.h"
#include "RoomStart.h"

RMesh* RoomStart::baseRMesh = nullptr;

void RoomStart::setBase(RMesh* inRme) {
	if (RoomStart::baseRMesh==nullptr) {
		RoomStart::baseRMesh = inRme;
	}
}

RoomStart* RoomStart::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomStart* retRoom = new RoomStart;

	retRoom->loadAssets(RoomStart::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomStart::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomStart::baseRMesh,startPos,destPos,posList);
}
