#include "../Room.h"
#include "Room2nuke.h"

RMesh* Room2nuke::baseRMesh = nullptr;

void Room2nuke::setBase(RMesh* inRme) {
	if (Room2nuke::baseRMesh==nullptr) {
		Room2nuke::baseRMesh = inRme;
	}
}

Room2nuke* Room2nuke::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2nuke* retRoom = new Room2nuke;

	retRoom->loadAssets(Room2nuke::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2nuke::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2nuke::baseRMesh,startPos,destPos,posList);
}
