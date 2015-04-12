#include "../Room.h"
#include "RoomTunnel2.h"

RMesh* RoomTunnel2::baseRMesh = nullptr;

void RoomTunnel2::setBase(RMesh* inRme) {
	if (RoomTunnel2::baseRMesh==nullptr) {
		RoomTunnel2::baseRMesh = inRme;
	}
}

RoomTunnel2* RoomTunnel2::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomTunnel2* retRoom = new RoomTunnel2;

	retRoom->loadAssets(RoomTunnel2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomTunnel2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomTunnel2::baseRMesh,startPos,destPos,posList);
}
