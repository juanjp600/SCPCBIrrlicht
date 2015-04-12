#include "../Room.h"
#include "Room2testRoom2.h"

RMesh* Room2testRoom2::baseRMesh = nullptr;

void Room2testRoom2::setBase(RMesh* inRme) {
	if (Room2testRoom2::baseRMesh==nullptr) {
		Room2testRoom2::baseRMesh = inRme;
	}
}

Room2testRoom2* Room2testRoom2::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2testRoom2* retRoom = new Room2testRoom2;

	retRoom->loadAssets(Room2testRoom2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2testRoom2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2testRoom2::baseRMesh,startPos,destPos,posList);
}
