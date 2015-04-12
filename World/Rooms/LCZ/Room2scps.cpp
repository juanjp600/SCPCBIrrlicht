#include "../Room.h"
#include "Room2scps.h"

RMesh* Room2scps::baseRMesh = nullptr;

void Room2scps::setBase(RMesh* inRme) {
	if (Room2scps::baseRMesh==nullptr) {
		Room2scps::baseRMesh = inRme;
	}
}

Room2scps* Room2scps::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2scps* retRoom = new Room2scps;

	retRoom->loadAssets(Room2scps::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2scps::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2scps::baseRMesh,startPos,destPos,posList);
}
