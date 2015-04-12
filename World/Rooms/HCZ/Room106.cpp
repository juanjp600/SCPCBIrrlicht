#include "../Room.h"
#include "Room106.h"

RMesh* Room106::baseRMesh = nullptr;

void Room106::setBase(RMesh* inRme) {
	if (Room106::baseRMesh==nullptr) {
		Room106::baseRMesh = inRme;
	}
}

Room106* Room106::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room106* retRoom = new Room106;

	retRoom->loadAssets(Room106::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room106::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room106::baseRMesh,startPos,destPos,posList);
}
