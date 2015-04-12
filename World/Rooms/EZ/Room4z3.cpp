#include "../Room.h"
#include "Room4z3.h"

RMesh* Room4z3::baseRMesh = nullptr;

void Room4z3::setBase(RMesh* inRme) {
	if (Room4z3::baseRMesh==nullptr) {
		Room4z3::baseRMesh = inRme;
	}
}

Room4z3* Room4z3::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room4z3* retRoom = new Room4z3;

	retRoom->loadAssets(Room4z3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room4z3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room4z3::baseRMesh,startPos,destPos,posList);
}
