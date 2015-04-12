#include "../Room.h"
#include "Room2toilets.h"

RMesh* Room2toilets::baseRMesh = nullptr;

void Room2toilets::setBase(RMesh* inRme) {
	if (Room2toilets::baseRMesh==nullptr) {
		Room2toilets::baseRMesh = inRme;
	}
}

Room2toilets* Room2toilets::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2toilets* retRoom = new Room2toilets;

	retRoom->loadAssets(Room2toilets::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2toilets::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2toilets::baseRMesh,startPos,destPos,posList);
}
