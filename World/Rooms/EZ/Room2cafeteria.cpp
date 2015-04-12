#include "../Room.h"
#include "Room2cafeteria.h"

RMesh* Room2cafeteria::baseRMesh = nullptr;

void Room2cafeteria::setBase(RMesh* inRme) {
	if (Room2cafeteria::baseRMesh==nullptr) {
		Room2cafeteria::baseRMesh = inRme;
	}
}

Room2cafeteria* Room2cafeteria::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2cafeteria* retRoom = new Room2cafeteria;

	retRoom->loadAssets(Room2cafeteria::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2cafeteria::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2cafeteria::baseRMesh,startPos,destPos,posList);
}
