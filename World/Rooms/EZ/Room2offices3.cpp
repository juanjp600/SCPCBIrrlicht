#include "../Room.h"
#include "Room2offices3.h"

RMesh* Room2offices3::baseRMesh = nullptr;

void Room2offices3::setBase(RMesh* inRme) {
	if (Room2offices3::baseRMesh==nullptr) {
		Room2offices3::baseRMesh = inRme;
	}
}

Room2offices3* Room2offices3::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2offices3* retRoom = new Room2offices3;

	retRoom->loadAssets(Room2offices3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2offices3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2offices3::baseRMesh,startPos,destPos,posList);
}
