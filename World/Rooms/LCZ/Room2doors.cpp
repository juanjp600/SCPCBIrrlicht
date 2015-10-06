#include "../Room.h"
#include "Room2doors.h"

#include "../../World.h"
#include "../Door.h"

RMesh* Room2doors::baseRMesh = nullptr;

void Room2doors::setBase(RMesh* inRme) {
	if (Room2doors::baseRMesh==nullptr) {
		Room2doors::baseRMesh = inRme;
	}
}

Room2doors* Room2doors::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room2doors* retRoom = new Room2doors;

	retRoom->loadAssets(Room2doors::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void Room2doors::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room2doors::baseRMesh,startPos,destPos,posList);
}

void Room2doors::updateEvent() {
    if (state==0) {
        doors[0]->open = true;
        doors[1]->open = false;
        state=1;
    } else if (state==1) {
        if (doors[0]->open==false) {
            if (doors[1]->open==false) {
                doors[1]->forceToggle();
                state=2;
            }
        } else if (doors[1]->open==true) {
            if (doors[0]->open==true) {
                doors[0]->forceToggle();
                state=2;
            }
        }
    } else if (state==2) {
        if (doors[1]->open==false) {
            if (doors[0]->open==false) {
                doors[0]->forceToggle();
                state=1;
            }
        } else if (doors[0]->open==true) {
            if (doors[1]->open==true) {
                doors[1]->forceToggle();
                state=1;
            }
        }
    }
}

void Room2doors::setDoor(unsigned char index,Door* door) {
    if (index>1) { return; }
    doors[index] = door;
}
