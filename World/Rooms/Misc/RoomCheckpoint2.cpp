#include "../Room.h"
#include "RoomCheckpoint2.h"

RMesh* RoomCheckpoint2::baseRMesh = nullptr;

void RoomCheckpoint2::setBase(RMesh* inRme) {
	if (RoomCheckpoint2::baseRMesh==nullptr) {
		RoomCheckpoint2::baseRMesh = inRme;
	}
}

RoomCheckpoint2* RoomCheckpoint2::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomCheckpoint2* retRoom = new RoomCheckpoint2;

	retRoom->loadAssets(RoomCheckpoint2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomCheckpoint2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomCheckpoint2::baseRMesh,startPos,destPos,posList);
}
