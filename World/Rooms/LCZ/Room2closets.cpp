#include "../Room.h"
#include "Room2closets.h"

RMesh* Room2closets::baseRMesh = nullptr;

void Room2closets::setBase(RMesh* inRme) {
	if (Room2closets::baseRMesh==nullptr) {
		Room2closets::baseRMesh = inRme;
	}
}

Room2closets* Room2closets::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2closets* retRoom = new Room2closets;

	retRoom->loadAssets(Room2closets::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2closets::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2closets::baseRMesh,startPos,destPos,posList);
}
