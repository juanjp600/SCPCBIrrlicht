#include "3dworld.h"
#include "rooms/rmesh.h"

#include <cmath>

void world::getRoomList(const irr::core::vector2di &startPos,const irr::core::vector2di &endPos,std::vector<irr::core::vector2di> &roomPath) {
	if (roomArray[startPos.X][startPos.Y]==nullptr) { std::cout<<"err1\n"; return; }
	if (roomArray[endPos.X][endPos.Y]==nullptr) { std::cout<<"err2\n"; return; }
	roomPath.clear();

    irr::core::vector2di sPos = startPos;

	unsigned char startDir = 4;
	unsigned char startDist = 0;

	if (roomArray[startPos.X][startPos.Y]->getType()==roomTypes::ROOM1 || roomArray[startPos.X][startPos.Y]->getType()==roomTypes::ROOM2) {
		unsigned char dist1 = 1;
		while (roomArray[startPos.X+dist1][startPos.Y]!=nullptr) {
			if (roomArray[startPos.X+dist1][startPos.Y]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X+dist1][startPos.Y]->getType()!=roomTypes::ROOM2) {
				startDir = 0;
				startDist = dist1;
				break;
			}
			dist1++;
		}
		dist1 = 1;
		while (roomArray[startPos.X][startPos.Y-dist1]!=nullptr) {
			if (roomArray[startPos.X][startPos.Y-dist1]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X][startPos.Y-dist1]->getType()!=roomTypes::ROOM2) {
				if (startDist>dist1 || startDist==0) {
					startDir = 1;
					startDist = dist1;
				}
				break;
			}
			dist1++;
		}
		dist1 = 1;
		while (roomArray[startPos.X-dist1][startPos.Y]!=nullptr) {
			if (roomArray[startPos.X-dist1][startPos.Y]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X-dist1][startPos.Y]->getType()!=roomTypes::ROOM2) {
				if (startDist>dist1 || startDist==0) {
					startDir = 2;
					startDist = dist1;
				}
				break;
			}
			dist1++;
		}
		dist1 = 1;
		while (roomArray[startPos.X][startPos.Y+dist1]!=nullptr) {
			if (roomArray[startPos.X][startPos.Y+dist1]->getType()!=roomTypes::ROOM1 && roomArray[startPos.X][startPos.Y+dist1]->getType()!=roomTypes::ROOM2) {
				if (startDist>dist1 || startDist==0) {
					startDir = 3;
					startDist = dist1;
				}
				break;
			}
			dist1++;
		}
		switch (startDir) {
            case 0:
                sPos.X+=startDist;
            break;
            case 1:
                sPos.Y-=startDist;
            break;
            case 2:
                sPos.X-=startDist;
            break;
            case 3:
                sPos.Y+=startDist;
            break;
            default:
                //return;
            break;
		}
	}

	tempPathList* testList = new tempPathList;
    testList->x = sPos.X;
    testList->y = sPos.Y;
    if (roomArray[sPos.X][sPos.Y]->getType()==roomTypes::ROOM1 || roomArray[sPos.X][sPos.Y]->getType()==roomTypes::ROOM2) {
        //std::cout<<"error at start\n";
    }
    testList->prev = nullptr;
    testList->next = nullptr;

    //detect if the first tempPathList should be removed
    switch (startDir) {
        case 0:
            if (testList->next!=nullptr) {
                if (testList->next->x < testList->x) {
                    tempPathList* tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        break;
        case 1:
            if (testList->next!=nullptr) {
                if (testList->next->y > testList->y) {
                    tempPathList* tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        break;
        case 2:
            if (testList->next!=nullptr) {
                if (testList->next->x > testList->x) {
                    tempPathList* tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        break;
        case 3:
            if (testList->next!=nullptr) {
                if (testList->next->y < testList->y) {
                    tempPathList* tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        break;
    }

    short result = stepPath(endPos,*testList);
    if (result>=0) {
        while (testList!=nullptr) {
            roomPath.push_back(irr::core::vector2di(testList->x,testList->y));
            tempPathList* tempList = testList;
            testList = testList->next;
            delete tempList;
        }
    }
}

short world::stepPath(const irr::core::vector2di &endPos,tempPathList &roomPath,int depth) {
    ////std::cout<<"asdasdasda"<<depth<<"\n";
	short minDist = -1;

    unsigned char x=roomPath.x;
    unsigned char y=roomPath.y;

    if (roomArray[x][y]->getType()==roomTypes::ROOM1 || roomArray[x][y]->getType()==roomTypes::ROOM2) {
        //std::cout<<"something is wrong with this\n";
    }

    //check if endPos can be reached directly from (x,y)
	if (x==endPos.X) {
        //std::cout<<"testX\n";
        if (y-endPos.Y==0) {
            return 0;
        } else if (y-endPos.Y<0) {
            for (unsigned short iy=y;iy<=endPos.Y;iy++) {
                if (roomArray[x][iy]==nullptr) { break; }
                if (iy==endPos.Y) { return endPos.Y-y; }
            }
            //if (roomArray[x][y]->getLinkedTurnDist(3)>=endPos.Y-y && roomArray[x][y]->getLinkedTurnDist(1)>0) { //std::cout<<"www\n"; return endPos.Y-y; }
        } else /* if (y-endPos.Y>0) */ {
            for (unsigned short iy=endPos.Y;iy<=y;iy++) {
                if (roomArray[x][iy]==nullptr) { break; }
                if (iy==y) { return y-endPos.Y; }
            }
            //if (roomArray[x][y]->getLinkedTurnDist(1)>=y-endPos.Y && roomArray[x][y]->getLinkedTurnDist(3)>0) { //std::cout<<"sss\n"; return y-endPos.Y; }
        }
	} else if (y==endPos.Y) {
        //std::cout<<"testY\n";
        if (x-endPos.X==0) {
            return 0;
        } else if (x-endPos.X<0) {
            for (unsigned short ix=x;ix<=endPos.X;ix++) {
                if (roomArray[ix][y]==nullptr) { break; }
                if (ix==endPos.X) { return endPos.X-x; }
            }
            //if (roomArray[x][y]->getLinkedTurnDist(0)>=endPos.X-x && roomArray[x][y]->getLinkedTurnDist(2)>0) { //std::cout<<"aaa\n"; return endPos.X-x; }
        } else /* if (x-endPos.X>0) */ {
            for (unsigned short ix=endPos.X;ix<=x;ix++) {
                if (roomArray[ix][y]==nullptr) { break; }
                if (ix==x) { return x-endPos.X; }
            }
            //if (roomArray[x][y]->getLinkedTurnDist(2)>=x-endPos.X && roomArray[x][y]->getLinkedTurnDist(0)>0) { //std::cout<<"ddd\n"; return x-endPos.X; }
        }
	}

    //check if visible "turns" can see endPos
    tempPathList* newList = nullptr;
    tempPathList* testList = nullptr;

	short rDist = roomArray[x][y]->getLinkedTurnDist(0);
	if (rDist>0) {
		bool isntInList = true;
        tempPathList* tempList = &roomPath;
        while (tempList!=nullptr) {
            if (tempList->x==x+rDist && tempList->y==y) {
                isntInList = false;
                break;
            }
            tempList=tempList->prev;
        }
        if (isntInList) {
            tempList = roomPath.next;
            while (tempList!=nullptr) {
                if (tempList->x==x+rDist && tempList->y==y) {
                    isntInList = false;
                    break;
                }
                tempList=tempList->next;
            }
        }
		if (isntInList) {
            testList = new tempPathList;
            testList->x = x+rDist;
            testList->y = y;
            testList->prev = &roomPath;
            short dist = stepPath(endPos,*testList,depth+1);
            if ((dist+rDist<minDist || minDist<=0) && dist>=0) {
                minDist = dist+rDist;
                while (newList!=nullptr) {
                    tempList = newList;
                    newList = newList->next;
                    delete tempList;
                }
                newList = testList;
            } else {
                while (testList!=nullptr) {
                    tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        }
	}
	rDist = roomArray[x][y]->getLinkedTurnDist(1);
	if (rDist>0) {
		bool isntInList = true;
        tempPathList* tempList = &roomPath;
        while (tempList!=nullptr) {
            if (tempList->x==x && tempList->y==y-rDist) {
                isntInList = false;
                break;
            }
            tempList=tempList->prev;
        }
        if (isntInList) {
            tempList = roomPath.next;
            while (tempList!=nullptr) {
                if (tempList->x==x && tempList->y==y-rDist) {
                    isntInList = false;
                    break;
                }
                tempList=tempList->next;
            }
        }
		if (isntInList) {
            testList = new tempPathList;
            testList->x = x;
            testList->y = y-rDist;
            testList->prev = &roomPath;
            short dist = stepPath(endPos,*testList,depth+1);
            if ((dist+rDist<minDist || minDist<=0) && dist>=0) {
                minDist = dist+rDist;
                while (newList!=nullptr) {
                    tempList = newList;
                    newList = newList->next;
                    delete tempList;
                }
                newList = testList;
            } else {
                while (testList!=nullptr) {
                    tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        }
	}
	rDist = roomArray[x][y]->getLinkedTurnDist(2);
	if (rDist>0) {
		bool isntInList = true;
        tempPathList* tempList = &roomPath;
        while (tempList!=nullptr) {
            if (tempList->x==x-rDist && tempList->y==y) {
                isntInList = false;
                break;
            }
            tempList=tempList->prev;
        }
        if (isntInList) {
            tempList = roomPath.next;
            while (tempList!=nullptr) {
                if (tempList->x==x-rDist && tempList->y==y) {
                    isntInList = false;
                    break;
                }
                tempList=tempList->next;
            }
        }
		if (isntInList) {
            testList = new tempPathList;
            testList->x = x-rDist;
            testList->y = y;
            testList->prev = &roomPath;
            short dist = stepPath(endPos,*testList,depth+1);
            if ((dist+rDist<minDist || minDist<=0) && dist>=0) {
                minDist = dist+rDist;
                while (newList!=nullptr) {
                    tempList = newList;
                    newList = newList->next;
                    delete tempList;
                }
                newList = testList;
            } else {
                while (testList!=nullptr) {
                    tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        }
	}
	rDist = roomArray[x][y]->getLinkedTurnDist(3);
	if (rDist>0) {
		bool isntInList = true;
        tempPathList* tempList = &roomPath;
        while (tempList!=nullptr) {
            if (tempList->x==x && tempList->y==y+rDist) {
                isntInList = false;
                break;
            }
            tempList=tempList->prev;
        }
        if (isntInList) {
            tempList = roomPath.next;
            while (tempList!=nullptr) {
                if (tempList->x==x && tempList->y==y+rDist) {
                    isntInList = false;
                    break;
                }
                tempList=tempList->next;
            }
        }
		if (isntInList) {
            testList = new tempPathList;
            testList->x = x;
            testList->y = y+rDist;
            testList->prev = &roomPath;
            short dist = stepPath(endPos,*testList,depth+1);
            if ((dist+rDist<minDist || minDist<=0) && dist>=0) {
                minDist = dist+rDist;
                while (newList!=nullptr) {
                    tempList = newList;
                    newList = newList->next;
                    delete tempList;
                }
                newList = testList;
            } else {
                while (testList!=nullptr) {
                    tempList = testList;
                    testList = testList->next;
                    delete tempList;
                }
            }
        }
	}

    if (newList!=nullptr) { roomPath.next = newList; }

    return minDist;
}
