#include "../Room.h"
#include "RoomEndroom2.h"

RMesh* RoomEndroom2::baseRMesh = nullptr;

void RoomEndroom2::setBase(RMesh* inRme) {
	if (RoomEndroom2::baseRMesh==nullptr) {
		RoomEndroom2::baseRMesh = inRme;
	}
}

RoomEndroom2* RoomEndroom2::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomEndroom2* retRoom = new RoomEndroom2;

	retRoom->loadAssets(RoomEndroom2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomEndroom2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomEndroom2::baseRMesh,startPos,destPos,posList);
}
