#include "../Room.h"
#include "Roompj.h"

RMesh* Roompj::baseRMesh = nullptr;

void Roompj::setBase(RMesh* inRme) {
	if (Roompj::baseRMesh==nullptr) {
		Roompj::baseRMesh = inRme;
	}
}

Roompj* Roompj::createNew(irr::core::vector3df inPosition,char inAngle) {
	Roompj* retRoom = new Roompj;

	retRoom->loadAssets(Roompj::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Roompj::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Roompj::baseRMesh,startPos,destPos,posList);
}
