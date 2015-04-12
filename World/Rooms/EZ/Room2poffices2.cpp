#include "../Room.h"
#include "Room2poffices2.h"

RMesh* Room2poffices2::baseRMesh = nullptr;

void Room2poffices2::setBase(RMesh* inRme) {
	if (Room2poffices2::baseRMesh==nullptr) {
		Room2poffices2::baseRMesh = inRme;
	}
}

Room2poffices2* Room2poffices2::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2poffices2* retRoom = new Room2poffices2;

	retRoom->loadAssets(Room2poffices2::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2poffices2::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2poffices2::baseRMesh,startPos,destPos,posList);
}
