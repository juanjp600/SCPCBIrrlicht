#include "../Room.h"
#include "Room4tunnels.h"

RMesh* Room4tunnels::baseRMesh = nullptr;

void Room4tunnels::setBase(RMesh* inRme) {
	if (Room4tunnels::baseRMesh==nullptr) {
		Room4tunnels::baseRMesh = inRme;
	}
}

Room4tunnels* Room4tunnels::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room4tunnels* retRoom = new Room4tunnels;

	retRoom->loadAssets(Room4tunnels::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room4tunnels::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room4tunnels::baseRMesh,startPos,destPos,posList);
}
