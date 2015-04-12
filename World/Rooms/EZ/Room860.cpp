#include "../Room.h"
#include "Room860.h"

RMesh* Room860::baseRMesh = nullptr;

void Room860::setBase(RMesh* inRme) {
	if (Room860::baseRMesh==nullptr) {
		Room860::baseRMesh = inRme;
	}
}

Room860* Room860::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room860* retRoom = new Room860;

	retRoom->loadAssets(Room860::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room860::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room860::baseRMesh,startPos,destPos,posList);
}
