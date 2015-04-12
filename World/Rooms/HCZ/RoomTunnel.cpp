#include "../Room.h"
#include "RoomTunnel.h"

RMesh* RoomTunnel::baseRMesh = nullptr;

void RoomTunnel::setBase(RMesh* inRme) {
	if (RoomTunnel::baseRMesh==nullptr) {
		RoomTunnel::baseRMesh = inRme;
	}
}

RoomTunnel* RoomTunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomTunnel* retRoom = new RoomTunnel;

	retRoom->loadAssets(RoomTunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomTunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomTunnel::baseRMesh,startPos,destPos,posList);
}
