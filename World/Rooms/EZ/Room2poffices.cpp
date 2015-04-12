#include "../Room.h"
#include "Room2poffices.h"

RMesh* Room2poffices::baseRMesh = nullptr;

void Room2poffices::setBase(RMesh* inRme) {
	if (Room2poffices::baseRMesh==nullptr) {
		Room2poffices::baseRMesh = inRme;
	}
}

Room2poffices* Room2poffices::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2poffices* retRoom = new Room2poffices;

	retRoom->loadAssets(Room2poffices::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2poffices::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2poffices::baseRMesh,startPos,destPos,posList);
}
