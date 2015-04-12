#include "../Room.h"
#include "RoomCheckpoint1.h"

RMesh* RoomCheckpoint1::baseRMesh = nullptr;

void RoomCheckpoint1::setBase(RMesh* inRme) {
	if (RoomCheckpoint1::baseRMesh==nullptr) {
		RoomCheckpoint1::baseRMesh = inRme;
	}
}

RoomCheckpoint1* RoomCheckpoint1::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomCheckpoint1* retRoom = new RoomCheckpoint1;

	retRoom->loadAssets(RoomCheckpoint1::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomCheckpoint1::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomCheckpoint1::baseRMesh,startPos,destPos,posList);
}
