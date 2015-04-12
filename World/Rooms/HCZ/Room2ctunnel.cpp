#include "../Room.h"
#include "Room2ctunnel.h"

RMesh* Room2ctunnel::baseRMesh = nullptr;

void Room2ctunnel::setBase(RMesh* inRme) {
	if (Room2ctunnel::baseRMesh==nullptr) {
		Room2ctunnel::baseRMesh = inRme;
	}
}

Room2ctunnel* Room2ctunnel::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2ctunnel* retRoom = new Room2ctunnel;

	retRoom->loadAssets(Room2ctunnel::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2ctunnel::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2ctunnel::baseRMesh,startPos,destPos,posList);
}
