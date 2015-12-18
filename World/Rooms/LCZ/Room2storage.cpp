#include "../Room.h"
#include "../../World.h"
#include "../../Player.h"
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

    retRoom->tpPos1S = irr::core::vector3df(-76.f*RoomScale,16.f*RoomScale,0.f);
	retRoom->tpPos1D = irr::core::vector3df(26.4f*RoomScale,16.f*RoomScale,0.f);
	retRoom->tpPos2S = irr::core::vector3df(76.f*RoomScale,16.f*RoomScale,0.f);
	retRoom->tpPos2D = irr::core::vector3df(-26.4f*RoomScale,16.f*RoomScale,0.f);

	retRoom->rotMatrix.transformVect(retRoom->tpPos1S); retRoom->tpPos1S+= inPosition;
	retRoom->rotMatrix.transformVect(retRoom->tpPos1D); retRoom->tpPos1D+= inPosition;
	retRoom->rotMatrix.transformVect(retRoom->tpPos2S); retRoom->tpPos2S+= inPosition;
	retRoom->rotMatrix.transformVect(retRoom->tpPos2D); retRoom->tpPos2D+= inPosition;

	retRoom->tpBBox1.MinEdge = irr::core::vector3df(-76.f*RoomScale,0.f*RoomScale,-15.f*RoomScale);
	retRoom->tpBBox1.MaxEdge = irr::core::vector3df(-100.f*RoomScale,32.f*RoomScale,15.f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->tpBBox1.MinEdge);
	retRoom->rotMatrix.transformVect(retRoom->tpBBox1.MaxEdge);
	retRoom->tpBBox1.repair();
	retRoom->tpBBox1.MinEdge+= inPosition;
	retRoom->tpBBox1.MaxEdge+= inPosition;

	retRoom->tpBBox2.MinEdge = irr::core::vector3df(76.f*RoomScale,0.f*RoomScale,-15.f*RoomScale);
	retRoom->tpBBox2.MaxEdge = irr::core::vector3df(100.f*RoomScale,32.f*RoomScale,15.f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->tpBBox2.MinEdge);
	retRoom->rotMatrix.transformVect(retRoom->tpBBox2.MaxEdge);
	retRoom->tpBBox2.repair();
	retRoom->tpBBox2.MinEdge+= inPosition;
	retRoom->tpBBox2.MaxEdge+= inPosition;
	return retRoom;
}

void Room2storage::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2storage::baseRMesh,startPos,destPos,posList);
}

void Room2storage::updateEvent() {
    if (tpBBox1.isPointInside(GameObject::mainPlayer->getPosition())) {
        GameObject::mainPlayer->teleport(GameObject::mainPlayer->getPosition()-tpPos1S+tpPos1D);
        GameObject::mainPlayer->updateHead();
        GameObject::mainPlayer->camera->captureState();
    }
    if (tpBBox2.isPointInside(GameObject::mainPlayer->getPosition())) {
        GameObject::mainPlayer->teleport(GameObject::mainPlayer->getPosition()-tpPos2S+tpPos2D);
        GameObject::mainPlayer->updateHead();
        GameObject::mainPlayer->camera->captureState();
    }
}

void Room2storage::setDoor(unsigned char index,Door* door) {
    if (index>3) { return; }
    doors[index] = door;
}
