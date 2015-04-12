#include "../Room.h"
#include "Room2ccont.h"

RMesh* Room2ccont::baseRMesh = nullptr;

void Room2ccont::setBase(RMesh* inRme) {
	if (Room2ccont::baseRMesh==nullptr) {
		Room2ccont::baseRMesh = inRme;
	}
}

Room2ccont* Room2ccont::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2ccont* retRoom = new Room2ccont;

	retRoom->loadAssets(Room2ccont::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2ccont::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2ccont::baseRMesh,startPos,destPos,posList);
}
