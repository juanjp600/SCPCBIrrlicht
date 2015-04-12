#include "../Room.h"
#include "Room4.h"

RMesh* Room4::baseRMesh = nullptr;

void Room4::setBase(RMesh* inRme) {
	if (Room4::baseRMesh==nullptr) {
		Room4::baseRMesh = inRme;
	}
}

Room4* Room4::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room4* retRoom = new Room4;

	retRoom->loadAssets(Room4::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room4::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room4::baseRMesh,startPos,destPos,posList);
}
