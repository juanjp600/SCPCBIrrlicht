#include "../Room.h"
#include "RoomEndroom.h"

RMesh* RoomEndroom::baseRMesh = nullptr;

void RoomEndroom::setBase(RMesh* inRme) {
	if (RoomEndroom::baseRMesh==nullptr) {
		RoomEndroom::baseRMesh = inRme;
	}
}

RoomEndroom* RoomEndroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomEndroom* retRoom = new RoomEndroom;

	retRoom->loadAssets(RoomEndroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomEndroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomEndroom::baseRMesh,startPos,destPos,posList);
}
