#include "../Room.h"
#include "Room008.h"

RMesh* Room008::baseRMesh = nullptr;

void Room008::setBase(RMesh* inRme) {
	if (Room008::baseRMesh==nullptr) {
		Room008::baseRMesh = inRme;
	}
}

Room008* Room008::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room008* retRoom = new Room008;

	retRoom->loadAssets(Room008::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room008::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room008::baseRMesh,startPos,destPos,posList);
}
