#include "../Room.h"
#include "Room2pipes.h"

RMesh* Room2pipes::baseRMesh = nullptr;

void Room2pipes::setBase(RMesh* inRme) {
	if (Room2pipes::baseRMesh==nullptr) {
		Room2pipes::baseRMesh = inRme;
	}
}

Room2pipes* Room2pipes::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2pipes* retRoom = new Room2pipes;

	retRoom->loadAssets(Room2pipes::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2pipes::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2pipes::baseRMesh,startPos,destPos,posList);
}
