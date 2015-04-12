#include "../Room.h"
#include "Room2elevator.h"

RMesh* Room2elevator::baseRMesh = nullptr;

void Room2elevator::setBase(RMesh* inRme) {
	if (Room2elevator::baseRMesh==nullptr) {
		Room2elevator::baseRMesh = inRme;
	}
}

Room2elevator* Room2elevator::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2elevator* retRoom = new Room2elevator;

	retRoom->loadAssets(Room2elevator::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2elevator::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2elevator::baseRMesh,startPos,destPos,posList);
}
