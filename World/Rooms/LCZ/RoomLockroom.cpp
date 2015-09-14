#include "../Room.h"
#include "RoomLockroom.h"

#include "../../World.h"
#include "../Door.h"

RMesh* RoomLockroom::baseRMesh = nullptr;

void RoomLockroom::setBase(RMesh* inRme) {
	if (RoomLockroom::baseRMesh==nullptr) {
		RoomLockroom::baseRMesh = inRme;
	}
}

RoomLockroom* RoomLockroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomLockroom* retRoom = new RoomLockroom;

	retRoom->loadAssets(RoomLockroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomLockroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomLockroom::baseRMesh,startPos,destPos,posList);
}

void RoomLockroom::updateEvent() {
    if (state==0) {
        doors[0]->open = false;
        doors[1]->open = false;
        state=1;
        sounds[0] = Sound::getSound(std::string("SFX/Caution.ogg"),true,0);
    } else if (state==1) {
        if (doors[0]->open==true) {
            doors[1]->open=true;
            state=400;
        } else if (doors[1]->open==true) {
            doors[0]->open=true;
            state=400;
        }
    } else {
        if (doors[0]->open==false) {
            if (doors[1]->open==true) { doors[1]->forceToggle(); }
            state=1;
        } else if (doors[1]->open==false) {
            if (doors[0]->open==true) { doors[0]->forceToggle(); }
            state=1;
        } else {
            state--;
            if (state==80) {
                node->updateAbsolutePosition();
                sounds[0]->playSound(node->getAbsolutePosition(),100.f*RoomScale,1500.f*RoomScale,false,1.f);
            }
            if (state<=1) {
                if (doors[0]->open==true) { doors[0]->forceToggle(); }
                if (doors[1]->open==true) { doors[1]->forceToggle(); }
                state=1;
            }
        }
    }
}

void RoomLockroom::setDoor(unsigned char index,Door* door) {
    if (index>1) { return; }
    doors[index] = door;
}
