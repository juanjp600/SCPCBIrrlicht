#include "../Room.h"
#include "Room2_2.h"

RMesh* Room2_2::baseRMesh = nullptr;

void Room2_2::setBase(RMesh* inRme) {
	if (Room2_2::baseRMesh==nullptr) {
		Room2_2::baseRMesh = inRme;
	}
}

Room2_2* Room2_2::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2_2* retRoom = new Room2_2;

	retRoom->loadAssets(Room2_2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2_2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2_2::baseRMesh,startPos,destPos,posList);
}
