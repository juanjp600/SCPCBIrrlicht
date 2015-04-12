#include "../Room.h"
#include "RoomLockroom2.h"

RMesh* RoomLockroom2::baseRMesh = nullptr;

void RoomLockroom2::setBase(RMesh* inRme) {
	if (RoomLockroom2::baseRMesh==nullptr) {
		RoomLockroom2::baseRMesh = inRme;
	}
}

RoomLockroom2* RoomLockroom2::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomLockroom2* retRoom = new RoomLockroom2;

	retRoom->loadAssets(RoomLockroom2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomLockroom2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomLockroom2::baseRMesh,startPos,destPos,posList);
}
