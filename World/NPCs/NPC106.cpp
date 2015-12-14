#include "NPC106.h"
#include "../World.h"

#include <cmath>

irr::scene::IAnimatedMeshSceneNode* NPC106::baseNode = nullptr;

NPC106::NPC106() {
    node = NPC106::baseNode->clone();
}

NPC106* NPC106::createNPC106() {
    NPC106* returnNPC = new NPC106;
    return returnNPC;
}

void NPC106::teleport(irr::core::vector3df newPos) {
	node->setPosition(newPos);
}

void NPC106::update() {
    chasingPlayer = true;
    if (roomList.size()>0) {
        if (waypointList.size()>0) {
            boxNode->setPosition(waypointList[waypointList.size()-1]);
            if ((roomList[rListIndex].X==coordToRoomGrid(getPosition().X)) && (roomList[rListIndex].Y==coordToRoomGrid(getPosition().Z))) {
                currRoom=roomList[rListIndex];
            }
            if (currRoom==roomList[rListIndex]) {
                irr::core::vector3df wpDist = node->getPosition()-waypointList[wpListIndex];
                wpDist.Y=0.f;
                if (wpDist.getLength()<0.25f*speed.getLength()) {
                    wpListIndex++;
                    if (wpListIndex>=waypointList.size()) {
                        waypointList.clear(); std::cout<<"CLEAR1\n";
                        std::cout<<"106 this Room is done, go to next one\n";
                    } else {

                    }
                }
                if (waypointList.size()>0) {
                    node->updateAbsolutePosition();
                    speed = (waypointList[wpListIndex]-node->getAbsolutePosition()).normalize()*30.f;
                }
            } else {
                std::cout<<"here we go\n";
                node->updateAbsolutePosition();
                if (rListIndex>0) {
                    irr::core::vector3df doorPos = irr::core::vector3df(roomList[rListIndex].X*204.8f*RoomScale,5.f,roomList[rListIndex].Y*204.8f*RoomScale);
                    doorPos=(doorPos*0.51f)+irr::core::vector3df(roomList[rListIndex-1].X*204.8f*RoomScale,5.f,roomList[rListIndex-1].Y*204.8f*RoomScale)*0.49f;

                    speed = (doorPos-node->getAbsolutePosition()).normalize()*30.f;
                } else {
                    currRoom=roomList[rListIndex];
                }
            }
            searchTimer-=0.1f;
        }
        if (waypointList.size()==0) {
            speed = irr::core::vector3df(0.f,0.f,0.f);
            std::cout<<"106 waypointlist empty\n";
            wpListIndex = 0;
            rListIndex++;
            if (rListIndex>=roomList.size()) {
                roomList.clear();
                std::cout<<"106 end of search\n";
            } else {
                //currRoom = roomList[rListIndex];
                if (rListIndex<roomList.size()-1) {
                    owner->npcPathFind( getPosition(),//irr::core::vector3df((roomList[rListIndex-1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex-1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                        irr::core::vector3df((roomList[rListIndex+1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex+1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                        roomList[rListIndex],waypointList);
                    //searchTimer-=1.f;
                    if (waypointList.size()<=0) {
                        std::cout<<"error detected here!\n";
                    } else {
                        std::cout<<"waypointlist.size() = "<<waypointList.size()<<"\n";
                        for (int i=0;i<waypointList.size();i++) {
                            waypointList[i].Y+=5.f;
                        }
                    }
                } else {
                    owner->npcPathFindToPlayer( getPosition(),//irr::core::vector3df((roomList[rListIndex-1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex-1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                        roomList[rListIndex],waypointList);
                    //searchTimer-=1.f;
                    if (waypointList.size()<=0) {
                        std::cout<<"error detected here!\n";
                    } else {
                        std::cout<<"waypointlist.size() = "<<waypointList.size()<<"\n";
                        for (int i=0;i<waypointList.size();i++) {
                            waypointList[i].Y+=5.f;
                        }
                    }
                }
            }
        }
        std::cout<<"SEARCHTIMER:"<<searchTimer<<"\n";
        if (searchTimer<=0.f) {
            roomList.clear();
            //waypointList.clear(); std::cout<<"CLEAR2\n";
        }
    } else {
        speed = irr::core::vector3df(0.f,0.f,0.f);

        waypointList.clear(); std::cout<<"CLEAR3\n";
        wpListIndex = 0;
        rListIndex = 0;
        std::cout<<"106 Roomlist empty\n";

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
                } else if (tempRoomList[i].Y==tempRoomList[i+1].Y) {
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
                std::cout<<"error generating final 106 Roomlist: "<<(roomList[i].X-roomList[i+1].X)<<" "<<(roomList[i].Y-roomList[i+1].Y)<<"\n";
                mustFix = true;
                break;
            }
        }
        //if (mustFix) { std::terminate(); }
        std::cout<<" "<<tempRoomList.size()<<" vs "<<roomList.size()<<"\n";

        while (roomList[rListIndex].getDistanceFrom(roomList[roomList.size()-1])>6) {
            rListIndex++;
        }

        if ((irr::core::vector3df(roomList[rListIndex].X*204.8f*RoomScale,10.f,roomList[rListIndex].Y*204.8f*RoomScale)-(node->getPosition())).getLength()>205.f*RoomScale) {
            teleport(irr::core::vector3df(roomList[rListIndex].X*204.8f*RoomScale,10.f,roomList[rListIndex].Y*204.8f*RoomScale));
        }

        if (rListIndex<roomList.size()-2 && roomList.size()>1) {
            owner->npcPathFind( getPosition(),//irr::core::vector3df((roomList[rListIndex-1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex-1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                irr::core::vector3df((roomList[rListIndex+1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex+1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                roomList[rListIndex],waypointList);
        } else {
            owner->npcPathFindToPlayer( getPosition(),//irr::core::vector3df((roomList[rListIndex-1].X+roomList[rListIndex].X)*204.8f*RoomScale*0.5f,0,(roomList[rListIndex-1].Y+roomList[rListIndex].Y)*204.8f*RoomScale*0.5f),
                                roomList[rListIndex],waypointList);
        }

        for (int i=0;i<waypointList.size();i++) {
            waypointList[i].Y+=5.f;
        }

        searchTimer = 60.f;
    }
}

void NPC106::updateModel() {
    irr::core::vector3df rot;
    irr::core::vector3df pointAt;
    pointAt = speed;
    pointAt.Y = 0.f;
    if (pointAt.getLength()>30.f*RoomScale) {
        rot.Y = (std::atan2(pointAt.X,pointAt.Z)*irr::core::RADTODEG);

        while (rot.Y>180.f) {
            rot.Y-=360.f;
        }
        while (rot.Y<-180.f) {
            rot.Y+=360.f;
        }

        rot.Y = rot.Y-currAngle;

        while (rot.Y>180.f) {
            rot.Y-=360.f;
        }
        while (rot.Y<-180.f) {
            rot.Y+=360.f;
        }

        currAngle = currAngle+(rot.Y*0.1f);

        while (currAngle>180.f) {
            currAngle-=360.f;
        }
        while (currAngle<-180.f) {
            currAngle+=360.f;
        }
    }
    rot.Y = currAngle;
    node->setRotation(rot);
}
