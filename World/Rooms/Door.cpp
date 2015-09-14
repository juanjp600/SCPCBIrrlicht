#include "../../Sound/SoundWrapper.h"
#include "Door.h"
#include "../World.h"

irr::scene::IMeshSceneNode* Door::baseButtonNode[4] = {nullptr,nullptr,nullptr,nullptr};
irr::scene::IMeshSceneNode* Door::baseFrameNode = nullptr;
irrDynamics* Door::dynamics = nullptr;
irr::scene::IMeshSceneNode* Door::baseDoorNode[3] = {nullptr,nullptr,nullptr};
btBoxShape* Door::shape = nullptr;
Sound* Door::openSound[2][3] = {{nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr}};
Sound* Door::closeSound[2][3] = {{nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr}};

Door::Door() {}

Door* Door::createDoor(unsigned char inDoorType,unsigned char inKeycard,short inCode,unsigned char inScanner) {
    //0 = standard, 1 = hcz doors, 2 = blast doors
    Door* returnDoor = new Door;

    returnDoor->doorType = inDoorType;
    returnDoor->keycard = inKeycard;
    returnDoor->code = inCode;
    returnDoor->scanner = inScanner;

    returnDoor->doorNode1 = Door::baseDoorNode[returnDoor->doorType]->clone();
    returnDoor->doorNode2 = Door::baseDoorNode[returnDoor->doorType]->clone();

    returnDoor->buttonIndex = 0;
    if (returnDoor->keycard>0) { returnDoor->buttonIndex=1; }
    if (returnDoor->code>0) { returnDoor->buttonIndex=2; }
    if (returnDoor->scanner>0) { returnDoor->buttonIndex=3; }
    returnDoor->buttonNode1 = Door::baseButtonNode[returnDoor->buttonIndex]->clone();
    returnDoor->buttonNode2 = Door::baseButtonNode[returnDoor->buttonIndex]->clone();

    returnDoor->frameNode = nullptr;
    if (returnDoor->doorType<2) {
        returnDoor->frameNode = Door::baseFrameNode->clone();
    }

    if (returnDoor->frameNode!=nullptr) {
        returnDoor->collider = Door::dynamics->addBoxObject(returnDoor->frameNode,10000.f);
        returnDoor->collider->setLinearFactor(btVector3(0.f,0.f,0.f));
        returnDoor->collider->setAngularFactor(btVector3(0.f,0.f,0.f));
    }

    returnDoor->doorVisible1 = true;
    returnDoor->doorVisible2 = true;
    returnDoor->buttonVisible1 = true;
    returnDoor->buttonVisible2 = true;
    returnDoor->buttonOffset1 = irr::core::vector3df(-150.f*0.1f*RoomScale,165.f*0.1f*RoomScale,24.f*0.1f*RoomScale);
    returnDoor->buttonOffset2 = irr::core::vector3df(150.f*0.1f*RoomScale,165.f*0.1f*RoomScale,-24.f*0.1f*RoomScale);

    returnDoor->frameNode->updateAbsolutePosition();

    return returnDoor;
}

unsigned char Door::getButtonIndex() {
    return buttonIndex;
}

void Door::setPosition(irr::core::vector3df newPos) {
    irr::core::matrix4 rot;
    rot.setRotationDegrees(frameNode->getRotation());
    closedShift = irr::core::vector3df(0.f,0.f,8.f*0.1f*RoomScale);
    openShift = irr::core::vector3df(180.f*0.1f*RoomScale,0.f,8.f*0.1f*RoomScale);
    irr::core::vector3df buttonShiftVec1 = buttonOffset1;
    irr::core::vector3df buttonShiftVec2 = buttonOffset2;
    rot.transformVect(closedShift);
    rot.transformVect(openShift);
    rot.transformVect(buttonShiftVec1);
    rot.transformVect(buttonShiftVec2);

    doorNode1->setPosition(newPos+openShift*openState+closedShift*(1.f-openState));
    doorNode2->setPosition(newPos-openShift*openState-closedShift*(1.f-openState));
	frameNode->setPosition(newPos);
	buttonNode1->setPosition(newPos+buttonShiftVec1);
	buttonNode2->setPosition(newPos+buttonShiftVec2);
	btTransform rTransform = collider->getCenterOfMassTransform();
	rTransform.setOrigin(irrToBtVec(frameNode->getPosition()));
	collider->setCenterOfMassTransform(rTransform);
}

void Door::setRotation(float newAngle) {
    frameNode->setRotation(irr::core::vector3df(0.f,newAngle,0.f));
    doorNode1->setRotation(irr::core::vector3df(0.f,newAngle,0.f));
    doorNode2->setRotation(irr::core::vector3df(0.f,180.f+newAngle,0.f));
    buttonNode1->setRotation(irr::core::vector3df(0.f,180.f+newAngle,0.f));
    buttonNode2->setRotation(irr::core::vector3df(0.f,newAngle,0.f));
    btTransform rTransform = collider->getCenterOfMassTransform();
	rTransform.setRotation(btQuaternion(newAngle*irr::core::DEGTORAD,0.f,0.f));
	collider->setCenterOfMassTransform(rTransform);
    setPosition(frameNode->getPosition());
}

void Door::setButtonOffset(unsigned char index,irr::core::vector3df newOffset) {
    switch (index) {
        case 0:
            buttonOffset1 = newOffset;
        break;
        case 1:
            buttonOffset2 = newOffset;
        break;
    }
    setPosition(frameNode->getPosition());
}

void Door::setButtonVisibility(unsigned char index,bool newVisible) {
    switch (index) {
        case 0:
            buttonVisible1 = newVisible;
        break;
        case 1:
            buttonVisible2 = newVisible;
        break;
    }
}

void Door::setDoorVisibility(unsigned char index,bool newVisible) {
    switch (index) {
        case 0:
            doorVisible1 = newVisible;
        break;
        case 1:
            doorVisible2 = newVisible;
        break;
    }
}

void Door::update() {
    doorNode1->setVisible(doorVisible1);
    doorNode2->setVisible(doorVisible2);
    buttonNode1->setVisible(buttonVisible1);
    buttonNode2->setVisible(buttonVisible2);
    frameNode->setVisible(true);
    if (openState<1.f && open==true) {
        openState = std::min(1.f,openState+(0.005f*openState)+(0.025f*(1.f-openState)));
        doorNode1->setPosition(frameNode->getPosition()+openShift*openState+closedShift*(1.f-openState));
        doorNode2->setPosition(frameNode->getPosition()-openShift*openState-closedShift*(1.f-openState));
        //setPosition(frameNode->getPosition());
    } else if (openState>0.f && open==false) {
        openState = std::max(0.f,openState-(0.025f*openState)-(0.005f*(1.f-openState)));
        doorNode1->setPosition(frameNode->getPosition()+openShift*openState+closedShift*(1.f-openState));
        doorNode2->setPosition(frameNode->getPosition()-openShift*openState-closedShift*(1.f-openState));
        //setPosition(frameNode->getPosition());
    }
    if (openState>0.75f && isSolid==true) {
        dynamics->unregisterRBody(collider);
		isSolid = false;
    } else if (openState<0.75f && isSolid==false) {
        dynamics->registerNewRBody(frameNode,collider,-1,~0,~0,irr::core::vector3df(0,0.f,0));
		isSolid = true;
    }
}

void Door::disable() {
    if (isSolid==true) {
        dynamics->unregisterRBody(collider);
		isSolid = false;
		doorNode1->setVisible(false);
		doorNode2->setVisible(false);
		buttonNode1->setVisible(false);
		buttonNode2->setVisible(false);
		frameNode->setVisible(false);
    }
}

void Door::forceToggle() {
    if (open) {
        open = false;
        closeSound[0][rand()%3]->playSound(getPosition(),20.f,500.f);
    } else {
        open = true;
        openSound[0][rand()%3]->playSound(getPosition(),20.f,500.f);
    }
}

void Door::toggle(unsigned char inKeycard,short inCode,unsigned char inScanner) {
    if ((inKeycard>=keycard || keycard<=0) && (inCode==code || code<=0) && (inScanner==scanner || scanner<=0)) {
        if (openState>=0.99f && open) {
            open = false;
            closeSound[0][rand()%3]->playSound(getPosition(),20.f,500.f);
        } else if (openState<=0.01f) {
            open = true;
            openSound[0][rand()%3]->playSound(getPosition(),20.f,500.f);
        }
    }
}

irr::core::vector3df Door::getPosition() {
    return frameNode->getAbsolutePosition();
}

float Door::getRotation() {
    return frameNode->getAbsoluteTransformation().getRotationDegrees().Y;
}

irr::core::vector3df Door::getButtonPosition(unsigned char index) {
    switch (index) {
        case 0:
            buttonNode1->updateAbsolutePosition();
            return buttonNode1->getAbsolutePosition();
        break;
        case 1:
            buttonNode2->updateAbsolutePosition();
            return buttonNode2->getAbsolutePosition();
        break;
    }
    return irr::core::vector3df();
}

bool Door::getButtonVisibility(unsigned char index) {
    switch (index) {
        case 0:
            return buttonVisible1;
        break;
        case 1:
            return buttonVisible2;
        break;
    }
    return false;
}

float Door::getButtonRotation(unsigned char index) {
    switch (index) {
        case 0:
            return buttonNode1->getRotation().Y;
        break;
        case 1:
            return buttonNode2->getRotation().Y;
        break;
    }
    return 0.f;
}
