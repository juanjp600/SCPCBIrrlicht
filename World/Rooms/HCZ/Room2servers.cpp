#include "../Room.h"
#include "Room2servers.h"

RMesh* Room2servers::baseRMesh = nullptr;

void Room2servers::setBase(RMesh* inRme) {
	if (Room2servers::baseRMesh==nullptr) {
		Room2servers::baseRMesh = inRme;
	}
}

Room2servers* Room2servers::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2servers* retRoom = new Room2servers;

	retRoom->loadAssets(Room2servers::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2servers::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2servers::baseRMesh,startPos,destPos,posList);
}
