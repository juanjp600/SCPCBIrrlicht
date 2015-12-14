#include "../Room.h"
#include "Room2storage.h"

RMesh* Room2storage::baseRMesh = nullptr;

void Room2storage::setBase(RMesh* inRme) {
	if (Room2storage::baseRMesh==nullptr) {
		Room2storage::baseRMesh = inRme;
	}
}

Room2storage* Room2storage::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2storage* retRoom = new Room2storage;

	retRoom->loadAssets(Room2storage::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2storage::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2storage::baseRMesh,startPos,destPos,posList);
}

void Room2storage::updateEvent() {

}

void Room2storage::setDoor(unsigned char index,Door* door) {
    if (index>3) { return; }
    doors[index] = door;
}
