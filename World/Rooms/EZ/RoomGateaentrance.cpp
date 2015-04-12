#include "../Room.h"
#include "RoomGateaentrance.h"

RMesh* RoomGateaentrance::baseRMesh = nullptr;

void RoomGateaentrance::setBase(RMesh* inRme) {
	if (RoomGateaentrance::baseRMesh==nullptr) {
		RoomGateaentrance::baseRMesh = inRme;
	}
}

RoomGateaentrance* RoomGateaentrance::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomGateaentrance* retRoom = new RoomGateaentrance;

	retRoom->loadAssets(RoomGateaentrance::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomGateaentrance::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomGateaentrance::baseRMesh,startPos,destPos,posList);
}
