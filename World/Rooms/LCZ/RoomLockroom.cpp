#include "../Room.h"
#include "RoomLockroom.h"

RMesh* RoomLockroom::baseRMesh = nullptr;

void RoomLockroom::setBase(RMesh* inRme) {
	if (RoomLockroom::baseRMesh==nullptr) {
		RoomLockroom::baseRMesh = inRme;
	}
}

RoomLockroom* RoomLockroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomLockroom* retRoom = new RoomLockroom;

	retRoom->loadAssets(RoomLockroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomLockroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomLockroom::baseRMesh,startPos,destPos,posList);
}
