#include "../Room.h"
#include "Room2cz3.h"

RMesh* Room2cz3::baseRMesh = nullptr;

void Room2cz3::setBase(RMesh* inRme) {
	if (Room2cz3::baseRMesh==nullptr) {
		Room2cz3::baseRMesh = inRme;
	}
}

Room2cz3* Room2cz3::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2cz3* retRoom = new Room2cz3;

	retRoom->loadAssets(Room2cz3::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2cz3::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2cz3::baseRMesh,startPos,destPos,posList);
}
