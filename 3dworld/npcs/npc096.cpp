#include "npc096.h"
#include "../3dworld.h"

btCollisionShape* npc096::shape = nullptr;
//irr::scene::IMeshSceneNode* npc096::baseNode = nullptr;
irr::scene::ISceneNode* npc096::baseNode = nullptr;

npc096::npc096() {
    node = npc096::baseNode->clone();
}

npc096* npc096::createNPC096() {
    npc096* returnNPC = new npc096;
    return returnNPC;
}

void npc096::teleport(irr::core::vector3df newPos) {
    node->setPosition(newPos);
}

void npc096::update() {
    chasingPlayer = true;
    if (roomList.size()>0) {
        if (waypointList.size()>0) {
            if (node->getPosition().getDistanceFromSQ(waypointList[wpListIndex])<3.f*RoomScale) {
                wpListIndex++;
                if (wpListIndex>=waypointList.size()) {
                    waypointList.clear();
                    std::cout<<"096 this room is done, go to next one\n";
                } else {

                }
            }
            if (waypointList.size()>0) {
                node->updateAbsolutePosition();
                node->setPosition(node->getAbsolutePosition()+(waypointList[wpListIndex]-node->getAbsolutePosition()).normalize());
                //std::cout<<waypointList[wpListIndex].Y<<"\n";
            }
        } else {
            std::cout<<"096 waypointlist empty\n";
            wpListIndex = 0;
            rListIndex++;
            if (rListIndex>=roomList.size()) {
                roomList.clear();
                std::cout<<"096 end of search\n";
            } else {
                currRoom = roomList[rListIndex];
                if (rListIndex<roomList.size()-1) {
                    owner->npcPathFind( irr::core::vector3df((roomList[rListIndex-1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex-1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                        irr::core::vector3df((roomList[rListIndex+1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex+1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                        currRoom,waypointList);
                    if (waypointList.size()<=0) {
                        std::cout<<"error detected here!\n";
                    } else {
                        std::cout<<"waypointlist.size() = "<<waypointList.size()<<"\n";
                    }
                } else {

                }
            }
        }
    } else {
        waypointList.clear();
        wpListIndex = 0;
        rListIndex = 0;
        std::cout<<"096 roomlist empty\n";
        currRoom = irr::core::vector2di(coordToRoomGrid(node->getPosition().X),coordToRoomGrid(node->getPosition().Z));
        std::vector<irr::core::vector2di> tempRoomList;
        owner->getRoomListToPlayer(currRoom,tempRoomList);
        for (unsigned int i=0;i<tempRoomList.size();i++) {
            roomList.push_back(tempRoomList[i]);
            if (i<tempRoomList.size()-1) {
                int add = 0;
                if (tempRoomList[i].X==tempRoomList[i+1].X) {
                    if (tempRoomList[i].Y<tempRoomList[i+1].Y) {
                        for (int j=tempRoomList[i].Y+1;j<tempRoomList[i+1].Y;j++) {
                            add++;
                            roomList.push_back(irr::core::vector2di(tempRoomList[i].X,tempRoomList[i].Y+add));
                        }
                    } else {
                        for (int j=tempRoomList[i+1].Y+1;j<tempRoomList[i].Y;j++) {
                            add++;
                            roomList.push_back(irr::core::vector2di(tempRoomList[i].X,tempRoomList[i].Y-add));
                        }
                    }
                } else {
                    if (tempRoomList[i].X<tempRoomList[i+1].X) {
                        for (int j=tempRoomList[i].X+1;j<tempRoomList[i+1].X;j++) {
                            add++;
                            roomList.push_back(irr::core::vector2di(tempRoomList[i].X+add,tempRoomList[i].Y));
                        }
                    } else {
                        for (int j=tempRoomList[i+1].X+1;j<tempRoomList[i].X;j++) {
                            add++;
                            roomList.push_back(irr::core::vector2di(tempRoomList[i].X-add,tempRoomList[i].Y));
                        }
                    }
                }
            }
        }
        bool mustFix = false;
        for (unsigned int i=0;i<roomList.size()-1;i++) {
            if (std::abs(roomList[i].X-roomList[i+1].X)>1 || std::abs(roomList[i].Y-roomList[i+1].Y)>1) {
                std::cout<<"error generating final 096 roomlist: "<<(roomList[i].X-roomList[i+1].X)<<" "<<(roomList[i].Y-roomList[i+1].Y)<<"\n";
                mustFix = true;
            }
        }
        if (mustFix) { std::terminate(); }
        std::cout<<" "<<tempRoomList.size()<<" vs "<<roomList.size()<<"\n";
        node->setPosition(irr::core::vector3df(roomList[0].X*204.8f*RoomScale,0.f,roomList[0].Y*204.8f*RoomScale));
    }
    /*if (chasingPlayer) {
        if (roomList.size()>0) {
            if (waypointList.size()>0) {
                if (node->getPosition().getDistanceFromSQ(waypointList[wpListIndex])<3.f*RoomScale) {
                    wpListIndex++;
                    if (wpListIndex>=waypointList.size()) {
                        waypointList.clear();
                        std::cout<<"096 this room is done, go to next one\n";
                    } else {

                    }
                }
                if (waypointList.size()>0) {
                    node->updateAbsolutePosition();
                    node->setPosition(node->getAbsolutePosition()+(waypointList[wpListIndex]-node->getAbsolutePosition()).normalize());
                    std::cout<<waypointList[wpListIndex].Y<<"\n";
                }
            } else if (rShift<rDist) {
                std::cout<<"adasdasdasqwew\n";
                rShift++;
                irr::core::vector2di prevCurrRoom = currRoom;
                switch (rDir) {
                    case 0:
                        currRoom.X++;
                    break;
                    case 1:
                        currRoom.Y--;
                    break;
                    case 2:
                        currRoom.X--;
                    break;
                    case 3:
                        currRoom.Y++;
                    break;
                }
                std::cout<<((currRoom-prevCurrRoom)).X<<" ... "<<((currRoom-prevCurrRoom)).Y<<"\n";
                owner->npcPathFind(node->getAbsolutePosition(),irr::core::vector3df((currRoom+(currRoom-prevCurrRoom)).X*204.8*RoomScale,0,(currRoom+(currRoom-prevCurrRoom)).Y*204.8*RoomScale),currRoom,waypointList);
                if (waypointList.size()<=0) {
                    std::cout<<"error detected here!11\n";
                    //std::terminate();
                }
                wpListIndex = 0;
            } else {
                std::cout<<"096 waypointlist empty\n";
                wpListIndex = 0;
                rListIndex++;
                if (rListIndex>=roomList.size()) {
                    roomList.clear();
                    std::cout<<"096 end of search\n";
                } else {
                    currRoom = roomList[rListIndex];
                    if (rListIndex<roomList.size()-1) {
                        rDist = std::max(std::abs(roomList[rListIndex].X-roomList[rListIndex+1].X),std::abs(roomList[rListIndex].Y-roomList[rListIndex+1].Y));
                        if (roomList[rListIndex].Y>roomList[rListIndex+1].Y) { rDir = 1; }
                        if (roomList[rListIndex+1].Y>roomList[rListIndex].Y) { rDir = 3; }
                        if (roomList[rListIndex].X>roomList[rListIndex+1].X) { rDir = 2; }
                        if (roomList[rListIndex+1].X>roomList[rListIndex].X) { rDir = 0; }
                        rShift = 0;
                        owner->npcPathFind(node->getPosition(),irr::core::vector3df(roomList[rListIndex+1].X,0,roomList[rListIndex+1].Y),currRoom,waypointList);
                        if (waypointList.size()<=0) {
                            std::cout<<"error detected here!\n";
                        }
                    } else {

                    }
                }
            }
        } else {
            waypointList.clear();
            wpListIndex = 0;
            rListIndex = 0;
            std::cout<<"096 roomlist empty\n";
            currRoom = irr::core::vector2di(coordToRoomGrid(node->getPosition().X),coordToRoomGrid(node->getPosition().Z));
            owner->getRoomListToPlayer(currRoom,roomList);
            node->setPosition(irr::core::vector3df(roomList[0].X*204.8f*RoomScale,0.f,roomList[0].Y*204.8f*RoomScale));
            if (roomList.size()>1) {
                rShift = 0;
                rDist = std::max(std::abs(roomList[0].X-roomList[1].X),std::abs(roomList[0].Y-roomList[1].Y));
                if (roomList[0].Y>roomList[1].Y) { rDir = 1; }
                if (roomList[1].Y>roomList[0].Y) { rDir = 3; }
                if (roomList[0].X>roomList[1].X) { rDir = 2; }
                if (roomList[1].X>roomList[0].X) { rDir = 0; }
            }
        }
    }*/
}
