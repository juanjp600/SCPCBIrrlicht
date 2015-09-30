#include "../Room.h"
#include "RoomStart.h"

#include "../../World.h"
#include "../Door.h"

RMesh* RoomStart::baseRMesh = nullptr;

void RoomStart::setBase(RMesh* inRme) {
	if (RoomStart::baseRMesh==nullptr) {
		RoomStart::baseRMesh = inRme;
	}
}

RoomStart* RoomStart::createNew(irr::core::vector3df inPosition,char inAngle) {
	RoomStart* retRoom = new RoomStart;

	retRoom->loadAssets(RoomStart::baseRMesh,inPosition,inAngle);
	return retRoom;
}

void RoomStart::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	Room::findWPPath(RoomStart::baseRMesh,startPos,destPos,posList);
}

void RoomStart::updateEvent() {
    if (state<850) {
        if (state==0) {
            sceneNodes[0]->setPosition(node->getAbsolutePosition()+irr::core::vector3df(0.f,0.f,-20.f*RoomScale));
            sceneNodes[1]->setPosition(node->getAbsolutePosition()+irr::core::vector3df(0.f,0.f,60.f*RoomScale));
            sceneNodes[1]->updateAbsolutePosition();

            sounds[0] = Sound::getSound(std::string("SFX/Intro/173scene1.ogg"),true,1);
            sndChannels[0] = sounds[0]->playSound(sceneNodes[1]->getAbsolutePosition(),10.f*RoomScale,250.f*RoomScale,false,0.75f);

            sceneNodes[1]->setRotation(irr::core::vector3df(-90.f,180.f,0.f));
            sceneNodes[1]->setCurrentFrame(1538.f);
        } else if (state>60 && state<=160) {
            float subRot = ((state-60)*0.01f);
            subRot = 1.f-subRot;
            subRot*=subRot;
            sceneNodes[1]->setRotation(irr::core::vector3df(-90.f,(subRot*180.f),0.f));
            sceneNodes[1]->setCurrentFrame(1538.f+((1.f-subRot)*14.f));
        }
        if (state>100) {
            sceneNodes[0]->setPosition(sceneNodes[0]->getPosition()+irr::core::vector3df(0.f,0.f,-0.275f*RoomScale));
            float currFrame = sceneNodes[0]->getFrameNr();

            if (currFrame>259.f) { currFrame=259.f; }
            currFrame-=0.3f;
            while (currFrame<235.f) { currFrame+=24.f; }
            sceneNodes[0]->setCurrentFrame(currFrame);
        }

        if (state>150) {
            sceneNodes[1]->setPosition(sceneNodes[1]->getPosition()+irr::core::vector3df(0.f,0.f,-0.35f*RoomScale));
            float currFrame = sceneNodes[1]->getFrameNr();

            if (currFrame>891.f) { currFrame=891.f; }
            currFrame-=0.6f;
            while (currFrame<847.f) { currFrame+=44.f; }
            sceneNodes[1]->setCurrentFrame(currFrame);

            if (sounds[0]->isPlaying()) {
                sounds[0]->moveSource(sndChannels[0],sceneNodes[1]->getAbsolutePosition());
            }
        }
        if (state<700) {
            doors[0]->open = false;
            doors[0]->openState = 0.f;
        } else if (doors[0]->open==false) {
            doors[0]->forceToggle();
            sceneNodes[0]->setVisible(false);
            sceneNodes[1]->setVisible(false);
        }

        state++;
    } else {
        if (sceneNodes[0]!=nullptr) {
            sceneNodes[0]->remove(); sceneNodes[0]=nullptr;
        }
        if (sceneNodes[1]!=nullptr) {
            sceneNodes[1]->remove(); sceneNodes[1]=nullptr;
        }
    }
}

void RoomStart::setIrrNode(unsigned char index,irr::scene::ISceneNode* node) {
    if (index>1) { return; }
    if (node->getType()!=irr::scene::ESNT_ANIMATED_MESH) { return; }
    sceneNodes[index] = static_cast<irr::scene::IAnimatedMeshSceneNode*>(node);
}

void RoomStart::setDoor(unsigned char index,Door* door) {
    if (index>1) { return; }
    doors[index] = door;
}
