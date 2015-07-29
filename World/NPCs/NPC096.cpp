#include "NPC096.h"
#include "../World.h"

#include <cmath>

btCollisionShape* NPC096::shape = nullptr;
//irr::scene::IMeshSceneNode* NPC096::baseNode = nullptr;
irr::scene::IAnimatedMeshSceneNode* NPC096::baseNode = nullptr;

NPC096::NPC096() {
    node = NPC096::baseNode->clone();
    collider = NPC::dynamics->addCapsuleObject(node,12.f*RoomScale/0.75f,3.f*RoomScale/0.75f,16000.f);
    NPC::dynamics->unregisterRBody(collider);
    NPC::dynamics->registerNewRBody(node,collider,-1,-1,~0,irr::core::vector3df(0.f,1.f,0.f));
    //collider->setLinearFactor(btVector3(0.f,0.f,0.f));
    collider->setAngularFactor(btVector3(0.f,0.f,0.f));
    collider->setGravity(btVector3(0.f,-100.f,0.f));
    screamLoop = Sound::getSound("SFX/096_4.ogg");
    screamChannel = screamLoop->playSound(irr::core::vector3df(0.f,0.f,0.f),2.f,500.f,true,0.5f);
}

NPC096* NPC096::createNPC096() {
    NPC096* returnNPC = new NPC096;
    return returnNPC;
}

void NPC096::teleport(irr::core::vector3df newPos) {
    btTransform oTrans = collider->getCenterOfMassTransform();
	oTrans.setOrigin(btVector3(newPos.X,newPos.Y,newPos.Z));
	collider->setCenterOfMassTransform(oTrans);
	collider->setLinearVelocity(btVector3(0,0,0));
	node->setPosition(newPos);
}

void NPC096::update() {
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
                if (wpDist.getLength()<0.25f*collider->getLinearVelocity().distance(btVector3())) {
                    wpListIndex++;
                    if (wpListIndex>=waypointList.size()) {
                        waypointList.clear(); std::cout<<"CLEAR1\n";
                        std::cout<<"096 this Room is done, go to next one\n";
                    } else {

                    }
                }
                if (waypointList.size()>0) {
                    node->updateAbsolutePosition();
                    collider->setLinearVelocity((irrToBtVec((waypointList[wpListIndex]-node->getAbsolutePosition()).normalize())*150.f));
                    collider->setFriction(0.f);
                    collider->setDamping(0.f,0.f);
                    collider->setRestitution(0.f);
                    collider->forceActivationState(ACTIVE_TAG); collider->activate();
                    //node->updateAbsolutePosition();
                    //node->setPosition(node->getAbsolutePosition()+(waypointList[wpListIndex]-node->getAbsolutePosition()).normalize());
                    //std::cout<<waypointList[wpListIndex].Y<<"\n";
                }
            } else {
                std::cout<<"here we go\n";
                node->updateAbsolutePosition();
                if (rListIndex>0) {
                    irr::core::vector3df doorPos = irr::core::vector3df(roomList[rListIndex].X*204.8f*RoomScale,5.f,roomList[rListIndex].Y*204.8f*RoomScale);
                    doorPos=(doorPos*0.51f)+irr::core::vector3df(roomList[rListIndex-1].X*204.8f*RoomScale,5.f,roomList[rListIndex-1].Y*204.8f*RoomScale)*0.49f;

                    collider->setLinearVelocity((irrToBtVec((doorPos-node->getAbsolutePosition()).normalize())*150.f));
                    collider->setFriction(0.f);
                    collider->setDamping(0.f,0.f);
                    collider->setRestitution(0.f);
                    collider->forceActivationState(ACTIVE_TAG); collider->activate();
                } else {
                    currRoom=roomList[rListIndex];
                }
            }
            searchTimer-=0.1f;
        }
        if (waypointList.size()==0) {
            collider->setLinearVelocity(btVector3(0,0,0));
            std::cout<<"096 waypointlist empty\n";
            wpListIndex = 0;
            rListIndex++;
            if (rListIndex>=roomList.size()) {
                roomList.clear();
                std::cout<<"096 end of search\n";
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
        collider->setLinearVelocity(btVector3(0,0,0));

        waypointList.clear(); std::cout<<"CLEAR3\n";
        wpListIndex = 0;
        rListIndex = 0;
        std::cout<<"096 Roomlist empty\n";

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
                std::cout<<"error generating final 096 Roomlist: "<<(roomList[i].X-roomList[i+1].X)<<" "<<(roomList[i].Y-roomList[i+1].Y)<<"\n";
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

void NPC096::updateModel() {
    irr::core::vector3df rot;
    irr::core::vector3df pointAt;
    pointAt = btToIrrVec(collider->getLinearVelocity());
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
    screamLoop->moveSource(screamChannel,node->getPosition());
}
