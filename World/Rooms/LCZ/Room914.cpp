#include "../Room.h"
#include "../../World.h"
#include "Room914.h"
#include "../Door.h"

RMesh* Room914::baseRMesh = nullptr;

void Room914::setBase(RMesh* inRme) {
	if (Room914::baseRMesh==nullptr) {
		Room914::baseRMesh = inRme;
	}
}

Room914* Room914::createNew(irr::core::vector3df inPosition,char inAngle) {
	Room914* retRoom = new Room914;

	retRoom->loadAssets(Room914::baseRMesh,inPosition,inAngle);

	retRoom->intake.MinEdge = irr::core::vector3df(-91.2f*RoomScale,43.2f*RoomScale,75.2f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->intake.MinEdge);
	retRoom->intake.MaxEdge = irr::core::vector3df(-51.2f*RoomScale,0.8f*RoomScale,54.4f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->intake.MaxEdge);
	retRoom->intake.repair();
	retRoom->output.MinEdge = irr::core::vector3df(91.2f*RoomScale,10.f*RoomScale,75.2f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->output.MinEdge);
	retRoom->output.MaxEdge = irr::core::vector3df(51.2f*RoomScale,0.8f*RoomScale,54.4f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->output.MaxEdge);
	retRoom->output.repair();
	retRoom->intake.MinEdge+=inPosition; retRoom->intake.MaxEdge+=inPosition;
	retRoom->output.MinEdge+=inPosition; retRoom->output.MaxEdge+=inPosition;

	retRoom->knobAngle1 = irr::core::vector3df(-0.5f,0.5f,0.f).normalize();
	retRoom->rotMatrix.transformVect(retRoom->knobAngle1);
	retRoom->knobAngle1 = retRoom->knobAngle1.getSphericalCoordinateAngles();
	if (retRoom->knobAngle1.X!=0.f) {
        if (retRoom->knobAngle1.Y<90.f) {
            retRoom->knobAngle1.Z = 83.f;
            retRoom->knobAngle2.Z = -83.f;
            retRoom->keyAngle1.Z = 180.f;
            retRoom->keyAngle2.Z = 0.f;
        } else {
            retRoom->knobAngle1.Z = -83.f;
            retRoom->knobAngle2.Z = 83.f;
            retRoom->keyAngle1.Z = -180.f;
            retRoom->keyAngle2.Z = 0.f;
        }
        retRoom->knobAngle1.X = 0.f;
        retRoom->knobAngle2.X = 0.f;
	} else if (retRoom->knobAngle1.Z!=0.f) {
        if (retRoom->knobAngle1.Y<90.f) {
            retRoom->knobAngle1.X = 83.f;
            retRoom->knobAngle2.X = -83.f;
            retRoom->keyAngle1.X = 180.f;
            retRoom->keyAngle2.X = 0.f;
        } else {
            retRoom->knobAngle1.X = -83.f;
            retRoom->knobAngle2.X = 83.f;
            retRoom->keyAngle1.X = -180.f;
            retRoom->keyAngle2.X = 0.f;
        }
        retRoom->knobAngle1.Z = 0.f;
        retRoom->knobAngle2.Z = 0.f;
	}
	retRoom->knobAngle1.Y = inAngle*90.f; retRoom->knobAngle2.Y = inAngle*90.f;
	retRoom->keyAngle1.Y = inAngle*90.f; retRoom->keyAngle2.Y = inAngle*90.f;
	retRoom->knobPosition = irr::core::vector3df(0.f,23.f*RoomScale,37.5f*RoomScale);
	retRoom->rotMatrix.transformVect(retRoom->knobPosition);
	retRoom->knobPosition+= inPosition;

	retRoom->sounds[0] = Sound::getSound(std::string("SFX/914snap.ogg"),true,1);
	retRoom->sounds[1] = Sound::getSound(std::string("SFX/Machine.ogg"),true,1);
	return retRoom;
}

void Room914::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(Room914::baseRMesh,startPos,destPos,posList);
}

void Room914::updateEvent() {
    sceneNodes[0]->setPosition(knobPosition);
    irr::core::vector3df knobSnappingRot = sceneNodes[0]->getRotation();
    sceneNodes[0]->setRotation(knobSlidingRot);

    float prevRot = 0.f;
    float d1 = knobSlidingRot.getDistanceFrom(knobAngle1);
    float d2 = knobSlidingRot.getDistanceFrom(knobAngle2);
    prevRot = d2/(d1+d2);

    if (state==0) {
        if (!Room::owner->mouseRotate(sceneNodes[0],knobAngle1,knobAngle2,false)) {
            knobSlidingRot = knobSnappingRot;
        } else {
            knobSlidingRot = sceneNodes[0]->getRotation();
        }
        if (!Room::owner->mouseRotate(sceneNodes[1],keyAngle1,keyAngle2,false)) {
            if (sceneNodes[1]->getRotation().getDistanceFromSQ(keyAngle1)<sceneNodes[1]->getRotation().getDistanceFromSQ(keyAngle2)) {
                state=1;
                sounds[1]->playSound(sceneNodes[0]->getPosition(),100.f*RoomScale,1500.f*RoomScale,false,0.5f);
            } else {
                sceneNodes[1]->setRotation((sceneNodes[1]->getRotation()+keyAngle2)*0.5f);
            }
        }
    } else {
        knobSlidingRot = knobSnappingRot;
        if (state<180) {
            doors[0]->open = true;
            doors[1]->open = false;
            sceneNodes[1]->setRotation((sceneNodes[1]->getRotation()+keyAngle1)*0.5f);
        } else if (state<600) {
            doors[0]->open = false;
            doors[1]->open = false;
            sceneNodes[1]->setRotation((sceneNodes[1]->getRotation()+keyAngle2)*0.5f);
        }
        state++;
        if (state>680) {

            Item::REFINE_SETTINGS setting;
            if (prevRot<0.125) {
                setting = Item::REFINE_SETTINGS::ROUGH;
            } else if (prevRot<0.375) {
                setting = Item::REFINE_SETTINGS::COARSE;
            } else if (prevRot<0.625) {
                setting = Item::REFINE_SETTINGS::RATIO1;
            } else if (prevRot<0.875) {
                setting = Item::REFINE_SETTINGS::FINE;
            } else {
                setting = Item::REFINE_SETTINGS::VERYFINE;
            }

            Room::owner->refineItems(setting,intake,output);

            doors[0]->open = true;
            doors[1]->open = true;
            state=0;
        }
    }

    float currentRot = 0.f;
    d1 = knobSnappingRot.getDistanceFrom(knobAngle1);
    d2 = knobSnappingRot.getDistanceFrom(knobAngle2);
    currentRot = d2/(d1+d2);
    float newRot = currentRot;
    d1 = knobSlidingRot.getDistanceFrom(knobAngle1);
    d2 = knobSlidingRot.getDistanceFrom(knobAngle2);
    currentRot = d2/(d1+d2);
    if (currentRot<0.125f) {
        newRot=newRot*0.5f;
        if (prevRot>=0.125f) {
            sounds[0]->playSound(sceneNodes[0]->getPosition(),100.f*RoomScale,1500.f*RoomScale,false,0.5f);
            knobSlidingRot = knobAngle1*0.f+knobAngle2*(1.f-0.f);
        }
    }
    else if (currentRot<0.375f) {
        newRot=(newRot+0.25f)*0.5f;
        if (prevRot<0.125f || prevRot>=0.375f) {
            sounds[0]->playSound(sceneNodes[0]->getPosition(),100.f*RoomScale,1500.f*RoomScale,false,0.5f);
            knobSlidingRot = knobAngle1*0.25f+knobAngle2*(1.f-0.25f);
        }
    }
    else if (currentRot<0.625f) {
        newRot=(newRot+0.5f)*0.5f;
        if (prevRot<0.375f || prevRot>=0.625f) {
            sounds[0]->playSound(sceneNodes[0]->getPosition(),100.f*RoomScale,1500.f*RoomScale,false,0.5f);
            knobSlidingRot = knobAngle1*0.5f+knobAngle2*(1.f-0.5f);
        }
    }
    else if (currentRot<0.875f) {
        newRot=(newRot+0.75f)*0.5f;
        if (prevRot<0.625f || prevRot>=0.875f) {
            sounds[0]->playSound(sceneNodes[0]->getPosition(),100.f*RoomScale,1500.f*RoomScale,false,0.5f);
            knobSlidingRot = knobAngle1*0.75f+knobAngle2*(1.f-0.75f);
        }
    }
    else {
        newRot=(newRot+1.f)*0.5f;
        if (prevRot<0.875f) {
            sounds[0]->playSound(sceneNodes[0]->getPosition(),100.f*RoomScale,1500.f*RoomScale,false,0.5f);
            knobSlidingRot = knobAngle1*1.f+knobAngle2*(1.f-1.f);
        }
    }
    sceneNodes[0]->setRotation(knobAngle1*newRot+knobAngle2*(1.f-newRot));
    sceneNodes[1]->setPosition(knobPosition-irr::core::vector3df(0.f,4.f*RoomScale,0.f));
}

void Room914::setIrrNode(unsigned char index,irr::scene::ISceneNode* node) {
    if (index>1) { return; }
    if (index==0) { node->setRotation((knobAngle1+knobAngle2)*0.5f); }
    if (index==1) { node->setRotation(keyAngle2); }
    //if (node->getType()!=irr::scene::ESNT_MESH) { return; }
    sceneNodes[index] = static_cast<irr::scene::IMeshSceneNode*>(node);
}

void Room914::setDoor(unsigned char index,Door* door) {
    if (index>2) { return; }
    doors[index] = door;
}
