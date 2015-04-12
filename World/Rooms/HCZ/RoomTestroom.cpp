#include "../Room.h"
#include "RoomTestroom.h"

RMesh* RoomTestroom::baseRMesh = nullptr;

void RoomTestroom::setBase(RMesh* inRme) {
	if (RoomTestroom::baseRMesh==nullptr) {
		RoomTestroom::baseRMesh = inRme;
	}
}

RoomTestroom* RoomTestroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomTestroom* retRoom = new RoomTestroom;

	retRoom->loadAssets(RoomTestroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomTestroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomTestroom::baseRMesh,startPos,destPos,posList);
}
