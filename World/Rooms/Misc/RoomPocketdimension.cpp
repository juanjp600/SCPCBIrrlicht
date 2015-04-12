#include "../Room.h"
#include "RoomPocketdimension.h"

RMesh* RoomPocketdimension::baseRMesh = nullptr;

void RoomPocketdimension::setBase(RMesh* inRme) {
	if (RoomPocketdimension::baseRMesh==nullptr) {
		RoomPocketdimension::baseRMesh = inRme;
	}
}

RoomPocketdimension* RoomPocketdimension::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomPocketdimension* retRoom = new RoomPocketdimension;

	retRoom->loadAssets(RoomPocketdimension::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomPocketdimension::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomPocketdimension::baseRMesh,startPos,destPos,posList);
}
