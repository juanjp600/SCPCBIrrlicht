#include "3dworld.h"
#include "rooms/rmesh.h"

#include <cmath>
#include <functional>

void world::getRoomList(const irr::core::vector2di &startPos,const irr::core::vector2di &endPos,std::vector<irr::core::vector2di> &roomPath) {
	if (roomArray[startPos.X][startPos.Y]==nullptr) return;
	if (roomArray[endPos.X][endPos.Y]==nullptr) return;
	roomPath.clear();

	unsigned char startDir = 4;
	unsigned char startDist = 0;
	if (roomArray[startPos.X][startPos.Y]->getType()==roomTypes::ROOM1 || roomArray[startPos.X][startPos.Y]->getType()!=roomTypes::ROOM2) {
		unsigned char dist1 = 0;
		while (roomArray[startPos.X+dist1][startPos.Y]!=nullptr) {
			dist1++;
			if (roomArray[startPos.X+dist1][startPos.Y]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X+dist1][startPos.Y]->getType()!=roomTypes::ROOM2) {
				startDir = 0;
				startDist = dist1;
				break;
			}
		}
		dist1 = 0;
		while (roomArray[startPos.X][startPos.Y-dist1]!=nullptr) {
			dist1++;
			if (roomArray[startPos.X][startPos.Y-dist1]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X][startPos.Y-dist1]->getType()!=roomTypes::ROOM2) {
				if (startDist>dist1) {
					startDir = 1;
					startDist = dist1;
				}
				break;
			}
		}
		dist1 = 0;
		while (roomArray[startPos.X-dist1][startPos.Y]!=nullptr) {
			dist1++;
			if (roomArray[startPos.X-dist1][startPos.Y]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X-dist1][startPos.Y]->getType()!=roomTypes::ROOM2) {
				if (startDist>dist1) {
					startDir = 2;
					startDist = dist1;
				}
				break;
			}
		}
		dist1 = 0;
		while (roomArray[startPos.X][startPos.Y+dist1]!=nullptr) {
			dist1++;
			if (roomArray[startPos.X][startPos.Y+dist1]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X][startPos.Y+dist1]->getType()!=roomTypes::ROOM2) {
				if (startDist>dist1) {
					startDir = 3;
					startDist = dist1;
				}
				break;
			}
		}
	} else {

	}
	if (startDir>=4) { return; }
}

short world::stepPath(const irr::core::vector2di &endPos,std::vector<irr::core::vector2di> &roomPath,unsigned char x,unsigned char y) {

	short minDist = -1;

	unsigned int arrayPos = roomPath.size();

	roomPath.push_back(irr::core::vector2di(x,y));

	if (x==endPos.X) {
		if (y>endPos.Y) {
			if (roomArray[x][y]->getLinkedTurnDist(1)>=y-endPos.Y) { return y-endPos.Y; }
		} else if (y<endPos.Y) {
			if (roomArray[x][y]->getLinkedTurnDist(3)>=endPos.Y-y) { return endPos.Y-y; }
		}
		return 0;
	} else if (y==endPos.Y) {
		if (x>endPos.X) {
			if (roomArray[x][y]->getLinkedTurnDist(0)>=x-endPos.X) { return x-endPos.X; }
		} else if (x<endPos.X) {
			if (roomArray[x][y]->getLinkedTurnDist(2)>=endPos.X-x) { return endPos.X-x; }
		}
		return 0;
	}

	short rDist = roomArray[x][y]->getLinkedTurnDist(0);
	if (rDist>0) {
		unsigned int arrayPos2 = roomPath.size();
		bool isntInList = true;
		for (unsigned int i=0;i<arrayPos;i++) {
			if (roomPath[i].X==x+rDist && roomPath[i].Y==y) { isntInList=false; break; }
		}
		if (isntInList) {
            short dist = stepPath(endPos,roomPath,x+rDist,y);
            if (dist<minDist || minDist<0) {
                minDist = dist;
                if (roomPath.size()-arrayPos2>0 && arrayPos2!=arrayPos) {
                    roomPath.erase(roomPath.begin()+arrayPos,roomPath.begin()+arrayPos2);
                }
            }
        }
	}
	rDist = roomArray[x][y]->getLinkedTurnDist(1);
	if (rDist>0) {
		unsigned int arrayPos2 = roomPath.size();
		short dist = stepPath(endPos,roomPath,x,y-rDist);
		if (dist<minDist || minDist<0) {
			minDist = dist;
			if (roomPath.size()-arrayPos2>0 && arrayPos2!=arrayPos) {
				roomPath.erase(roomPath.begin()+arrayPos,roomPath.begin()+arrayPos2);
			}
		}
	}
	rDist = roomArray[x][y]->getLinkedTurnDist(2);
	if (rDist>0) {
		unsigned int arrayPos2 = roomPath.size();
		short dist = stepPath(endPos,roomPath,x-rDist,y);
		if (dist<minDist || minDist<0) {
			minDist = dist;
			if (roomPath.size()-arrayPos2>0 && arrayPos2!=arrayPos) {
				roomPath.erase(roomPath.begin()+arrayPos,roomPath.begin()+arrayPos2);
			}
		}
	}
	rDist = roomArray[x][y]->getLinkedTurnDist(3);
	if (rDist>0) {
		unsigned int arrayPos2 = roomPath.size();
		short dist = stepPath(endPos,roomPath,x,y+rDist);
		if (dist<minDist || minDist<0) {
			minDist = dist;
			if (roomPath.size()-arrayPos2>0 && arrayPos2!=arrayPos) {
				roomPath.erase(roomPath.begin()+arrayPos,roomPath.begin()+arrayPos2);
			}
		}
	}

	return -1;
}
