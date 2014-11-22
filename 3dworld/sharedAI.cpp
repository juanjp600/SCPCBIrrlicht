#include "3dworld.h"
#include "rooms/rmesh.h"

#include <cmath>

void world::getRoomList(const irr::core::vector2di &startPos,const irr::core::vector2di &endPos,std::vector<irr::core::vector2di> &roomPath) {
	if (roomArray[startPos.X][startPos.Y]==nullptr) return;
	if (roomArray[endPos.X][endPos.Y]==nullptr) return;
	roomPath.clear();

	auto stepPath = [] (unsigned char x,unsigned char y) -> short {
		short minDist = -1;
		if (x=endPos.X) {
			if (y>endPos.Y) {
				if (roomArray[x][y]->getLinkedTurnDist(1)>=y-endPos.Y) { return y-endPos.Y; }
			} else if (y<endPos.Y) {
				if (roomArray[x][y]->getLinkedTurnDist(3)>=endPos.Y-y) { return endPos.Y-y; }
			} else {
				return 0;
			}
		} else if (y=endPos.Y) {
			if (x>endPos.X) {
				if (roomArray[x][y]->getLinkedTurnDist(0)>=x-endPos.X) { return x-endPos.X; }
			} else if (x<endPos.X) {
				if (roomArray[x][y]->getLinkedTurnDist(2)>=endPos.X-x) { return endPos.X-x; }
			} else {
				return 0;
			}
		}
	}

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
