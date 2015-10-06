#include "NPC178.h"
#include "../World.h"

#include <cmath>

btCollisionShape* NPC178::shape = nullptr;
//irr::scene::IMeshSceneNode* NPC178::baseNode = nullptr;
irr::scene::IAnimatedMeshSceneNode* NPC178::baseNode = nullptr;

NPC178::NPC178() {
    node = NPC178::baseNode->clone();
    collider = NPC::dynamics->addCapsuleObject(node,20.f*RoomScale,6.f*RoomScale,16000.f);
    NPC::dynamics->unregisterRBody(collider);
    NPC::dynamics->registerNewRBody(node,collider,-1,-1,~0,irr::core::vector3df(0.f,2.f,0.f));
    //collider->setLinearFactor(btVector3(0.f,0.f,0.f));
    collider->setAngularFactor(btVector3(0.f,0.f,0.f));
    collider->setGravity(btVector3(0.f,-100.f*RoomScale,0.f));
    memDir.X = rand()%500-250;
    memDir.Z = rand()%500-250;
    memDir = memDir.normalize();
    walkTimer = rand()%300+500;
    //static_cast<irr::scene::IAnimatedMeshSceneNode*>(node)->setFrameLoop(64,92);
    //static_cast<irr::scene::IAnimatedMeshSceneNode*>(node)->setAnimationSpeed(1.0f+(rand()%100));
    //static_cast<irr::scene::IAnimatedMeshSceneNode*>(node)->setJointMode(irr::scene::E_JOINT_UPDATE_ON_RENDER::EJUOR_NONE);
}

NPC178* NPC178::createNPC178() {
    NPC178* returnNPC = new NPC178;
    return returnNPC;
}

void NPC178::teleport(irr::core::vector3df newPos) {
    btTransform oTrans = collider->getCenterOfMassTransform();
	oTrans.setOrigin(btVector3(newPos.X,newPos.Y,newPos.Z));
	collider->setCenterOfMassTransform(oTrans);
	collider->setLinearVelocity(btVector3(0,0,0));
	node->setPosition(newPos);
}

void NPC178::update() {
    //chasingPlayer = true;
    if (chasingPlayer) {
        if (roomList.size()>0) {
            if (waypointList.size()>0) {
                //boxNode->setPosition(waypointList[waypointList.size()-1]);
                if ((roomList[rListIndex].X==coordToRoomGrid(getPosition().X)) && (roomList[rListIndex].Y==coordToRoomGrid(getPosition().Z))) {
                    currRoom=roomList[rListIndex];
                }
                if (currRoom==roomList[rListIndex]) {
                    irr::core::vector3df wpDist = node->getPosition()-waypointList[wpListIndex];
                    wpDist.Y=0.f;
                    std::cout<<wpDist.getLength()<<"\n";
                    if (wpDist.getLength()<std::max(4.5f*RoomScale*collider->getLinearVelocity().distance(btVector3()),6.f*RoomScale)) {
                        wpListIndex++;
                        if (wpListIndex>=waypointList.size()) {
                            waypointList.clear(); std::cout<<"CLEAR1\n";
                            std::cout<<"178 this Room is done, go to next one\n";
                        } else {
                            std::cout<<"178 next waypoint\n";
                        }
                    }
                    if (waypointList.size()>0) {
                        node->updateAbsolutePosition();
                        collider->setLinearVelocity((irrToBtVec((waypointList[wpListIndex]-node->getAbsolutePosition()).normalize())*50.f*RoomScale));
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
                        irr::core::vector3df doorPos = irr::core::vector3df(roomList[rListIndex].X*204.8f*RoomScale,1.f*RoomScale,roomList[rListIndex].Y*204.8f*RoomScale);
                        doorPos=(doorPos*0.51f)+irr::core::vector3df(roomList[rListIndex-1].X*204.8f*RoomScale,1.f*RoomScale,roomList[rListIndex-1].Y*204.8f*RoomScale)*0.49f;

                        collider->setLinearVelocity((irrToBtVec((doorPos-node->getAbsolutePosition()).normalize())*50.f*RoomScale));
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
                                waypointList[i].Y-=2.f*RoomScale;
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
                                waypointList[i].Y-=2.f*RoomScale;
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
            std::cout<<"178 Roomlist empty\n";

            currRoom = irr::core::vector2di(coordToRoomGrid(node->getPosition().X),coordToRoomGrid(node->getPosition().Z));
            std::vector<irr::core::vector2di> tempRoomList;
            tempRoomList.clear();
            owner->getRoomListToPlayer(currRoom,tempRoomList);
            if (tempRoomList.size()>0) {
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
                        std::cout<<"error generating final 178 Roomlist: "<<(roomList[i].X-roomList[i+1].X)<<" "<<(roomList[i].Y-roomList[i+1].Y)<<"\n";
                        mustFix = true;
                        break;
                    }
                }

                while (roomList[rListIndex].getDistanceFrom(roomList[roomList.size()-1])>6) {
                    rListIndex++;
                }

                if ((irr::core::vector3df(roomList[rListIndex].X*204.8f*RoomScale,10.f,roomList[rListIndex].Y*204.8f*RoomScale)-(node->getPosition())).getLength()>205.f*RoomScale) {
                    teleport(irr::core::vector3df(roomList[rListIndex].X*204.8f*RoomScale,10.f*RoomScale,roomList[rListIndex].Y*204.8f*RoomScale));
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
                    waypointList[i].Y-=2.f*RoomScale;
                }

            }
            //if (mustFix) { std::terminate(); }
            std::cout<<" "<<tempRoomList.size()<<" vs "<<roomList.size()<<"\n";

            searchTimer = 60.f;
        }
    } else {
        if (walkTimer>0.f) {
            if (rand()%30==1) {
                memDir+=irr::core::vector3df((float)(rand()%500)/2000.f,0.f,(float)(rand()%500)/2000.f);
                memDir = memDir.normalize();
            }
            float memDirAngle = (std::atan2(memDir.X,memDir.Z)*irr::core::RADTODEG)-currAngle;
            while (memDirAngle>180.f) {
                memDirAngle-=360.f;
            }
            while (memDirAngle<-180.f) {
                memDirAngle+=360.f;
            }

            irr::core::vector3df closeNormal = NPC::dynamics->rayTestNormal(collider->getCenterOfMassPosition()-btVector3(0,4.f*RoomScale,0),collider->getCenterOfMassPosition()-btVector3(0,4.f*RoomScale,0)+irrToBtVec(memDir*25.f*RoomScale));
            if (closeNormal.getLengthSQ()>0) {
                closeNormal = closeNormal.normalize();
                irr::core::vector3df reflectVector = (memDir - (2 * memDir.dotProduct(closeNormal)) * closeNormal).normalize();
                if ((memDir-reflectVector).getLengthSQ()<1.55f) {
                    std::cout<<"reflecting "<<(memDir-reflectVector).getLengthSQ()<<"\n";
                    memDir = reflectVector;
                    //currAngle = (std::atan2(memDir.X,memDir.Z)*irr::core::RADTODEG);
                    while (currAngle>180.f) {
                        currAngle-=360.f;
                    }
                    while (currAngle<-180.f) {
                        currAngle+=360.f;
                    }
                } else {
                    irr::core::vector3df perpindicular(closeNormal.Z,0.f,closeNormal.X);
                    perpindicular.normalize();
                    if ((perpindicular-reflectVector).getLengthSQ()<(-perpindicular-reflectVector).getLengthSQ()) {
                        memDir = perpindicular;
                    } else {
                        memDir = -perpindicular;
                    }
                }
            }

            //std::cout<<((prevPos-getPosition()).getLength()*60.f)<<" "<<(memDir*8.f*0.5f).getLength()<<"\n";

            if ((((prevPos-getPosition()).getLength()*60.f)<(memDir*8.f*0.5f).getLength())) {
                //std::cout<<"WHAT\n";
            }

            //if (std::abs(memDirAngle)>178.f) {
            if (((prevPos-getPosition()).getLength()*60.f<2.f) && skipStuckCheck<=0) { //ran into a wall, change direction randomly //NPC::dynamics->rayTest(collider->getCenterOfMassPosition(),collider->getCenterOfMassPosition()+irrToBtVec(memDir*5.5f))
                memDir.X = rand()%500-250;
                memDir.Z = rand()%500-250;
                memDir = memDir.normalize();
                skipStuckCheck = 0;
            }
            collider->setLinearVelocity(btVector3(memDir.X,0.f,memDir.Z)*8.f+btVector3(0,collider->getLinearVelocity()[1],0));
            collider->setFriction(0.f);
            collider->setDamping(0.f,0.f);
            collider->setRestitution(0.f);
            collider->forceActivationState(ACTIVE_TAG); collider->activate();
            skipStuckCheck--;
            //} else {
            //    collider->setLinearVelocity(btVector3(0,0,0));
            //    skipStuckCheck = 3;
            //}
        } else {
            collider->setLinearVelocity(btVector3(0,0,0));
            skipStuckCheck = 3;
            if (walkTimer<spinTimer) {
                memDir =irr::core::vector3df(rand()%500-250,0.f,rand()%500-250);
                memDir = memDir.normalize();
                spinTimer -= (rand()%150)+50;
            }
        }
        int prevWalkTimer = walkTimer;
        walkTimer-=1;

        irr::scene::IAnimatedMeshSceneNode* animNode = static_cast<irr::scene::IAnimatedMeshSceneNode*>(node);

        if (prevWalkTimer>=0 && walkTimer<0) {
            animNode->setCurrentFrame(206);
        }
        if (walkTimer<minWalkTimer) {
            animNode->setCurrentFrame(64);
            walkTimer = rand()%300+500;
            minWalkTimer = -(rand()%300+500);
            spinTimer = -(rand()%200);
        }

        if (walkTimer>0) {
            float currFrame = animNode->getFrameNr();

            if (currFrame<64.f) { currFrame=64.f; }
            currFrame+=0.55f;
            while (currFrame>92.f) { currFrame-=28.f; }
            animNode->setCurrentFrame(currFrame);
        } else {
            float currFrame = animNode->getFrameNr();

            if (currFrame<206.f) { currFrame=206.f; }
            currFrame+=0.1f;
            while (currFrame>240.f) { currFrame-=34.f; }
            animNode->setCurrentFrame(currFrame);
        }
    }
    prevPos = getPosition();
}

void NPC178::updateModel() {
    irr::core::vector3df rot;
    irr::core::vector3df pointAt;
    if (chasingPlayer) {
        pointAt = btToIrrVec(collider->getLinearVelocity());
    } else {
        pointAt = memDir*40.f*RoomScale;
    }
    pointAt.Y = 0.f;
    if (pointAt.getLength()>30.f*RoomScale) {
        rot.Y = (std::atan2(pointAt.X,pointAt.Z)*irr::core::RADTODEG)+180.f;

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

irr::core::vector3df NPC178::getPosition() {
    return btToIrrVec(collider->getCenterOfMassPosition());
}
