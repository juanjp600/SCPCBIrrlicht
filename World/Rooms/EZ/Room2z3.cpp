#include "../Room.h"
#include "Room2z3.h"

RMesh* Room2z3::baseRMesh = nullptr;

void Room2z3::setBase(RMesh* inRme) {
	if (Room2z3::baseRMesh==nullptr) {
		Room2z3::baseRMesh = inRme;
	}
}

Room2z3* Room2z3::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2z3* retRoom = new Room2z3;

	retRoom->loadAssets(Room2z3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2z3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2z3::baseRMesh,startPos,destPos,posList);
}
