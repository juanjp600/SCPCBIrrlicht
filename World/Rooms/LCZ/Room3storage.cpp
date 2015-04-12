#include "../Room.h"
#include "Room3storage.h"

RMesh* Room3storage::baseRMesh = nullptr;

void Room3storage::setBase(RMesh* inRme) {
	if (Room3storage::baseRMesh==nullptr) {
		Room3storage::baseRMesh = inRme;
	}
}

Room3storage* Room3storage::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room3storage* retRoom = new Room3storage;

	retRoom->loadAssets(Room3storage::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room3storage::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room3storage::baseRMesh,startPos,destPos,posList);
}
