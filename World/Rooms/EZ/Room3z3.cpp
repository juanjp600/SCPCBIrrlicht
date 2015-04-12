#include "../Room.h"
#include "Room3z3.h"

RMesh* Room3z3::baseRMesh = nullptr;

void Room3z3::setBase(RMesh* inRme) {
	if (Room3z3::baseRMesh==nullptr) {
		Room3z3::baseRMesh = inRme;
	}
}

Room3z3* Room3z3::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3z3* retRoom = new Room3z3;

	retRoom->loadAssets(Room3z3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3z3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3z3::baseRMesh,startPos,destPos,posList);
}
