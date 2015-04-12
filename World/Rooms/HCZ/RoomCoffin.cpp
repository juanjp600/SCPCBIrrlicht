#include "../Room.h"
#include "RoomCoffin.h"

RMesh* RoomCoffin::baseRMesh = nullptr;

void RoomCoffin::setBase(RMesh* inRme) {
	if (RoomCoffin::baseRMesh==nullptr) {
		RoomCoffin::baseRMesh = inRme;
	}
}

RoomCoffin* RoomCoffin::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomCoffin* retRoom = new RoomCoffin;

	retRoom->loadAssets(RoomCoffin::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomCoffin::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomCoffin::baseRMesh,startPos,destPos,posList);
}
