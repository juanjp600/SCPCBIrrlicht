#include "../Room.h"
#include "../../World.h"
#include "Room012.h"
#include "../Door.h"
#include "../../Player.h"

RMesh* Room012::baseRMesh = nullptr;

void Room012::setBase(RMesh* inRme) {
	if (Room012::baseRMesh==nullptr) {
		Room012::baseRMesh = inRme;
	}
}

Room012* Room012::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room012* retRoom = new Room012;

	retRoom->loadAssets(Room012::baseRMesh,inPosition,inAngle);

	retRoom->standPos = irr::core::vector3df(-37.f*RoomScale,-76.6f*RoomScale,30.f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->standPos);
	retRoom->standPos+= inPosition;
	return retRoom;
}

void Room012::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room012::baseRMesh,startPos,destPos,posList);
}

void Room012::updateEvent() {
    irr::core::vector3df pointADir = doors[0]->getPosition()-GameObject::mainPlayer->getPosition();
    float pointADist = pointADir.getLength();
    pointADir.Y = 0.f; pointADir.normalize();
    irr::core::vector3df pointBDir = standPos-GameObject::mainPlayer->getPosition();
    float pointBDist = pointBDir.getLength();
    pointBDir.Y = 0.f; pointBDir.normalize();
    if (pointADist<50.f*RoomScale || pointBDist<50.f*RoomScale) {
        std::cout<<pointADir.getDistanceFrom(pointBDir)<<"\n";
        if (pointADist>1.f*RoomScale && pointADir.getDistanceFrom(pointBDir)<1.3f) {
            irr::core::vector3df walkDir = pointADir;
            GameObject::mainPlayer->forceWalk(walkDir*0.8f,1.f);
        } else {
            irr::core::vector3df walkDir = pointBDir;
            GameObject::mainPlayer->forceWalk(walkDir*0.8f,1.f);
        }
    }
}

void Room012::setDoor(unsigned char index,Door* door) {
    if (index>1) { return; }
    doors[index] = door;
}
