#include "Door.h"
#include "../World.h"

irr::scene::IMeshSceneNode* Door::baseButtonNode[4] = {nullptr,nullptr,nullptr,nullptr};
irr::scene::IMeshSceneNode* Door::baseFrameNode = nullptr;
irrDynamics* Door::dynamics = nullptr;
irr::scene::IMeshSceneNode* Door::baseDoorNode[3] = {nullptr,nullptr,nullptr};

Door::Door() {}

Door* Door::createDoor(unsigned char doorType,unsigned char keycard,short code,unsigned char scanner) {
    //0 = standard, 1 = hcz doors, 2 = blast doors
    Door* returnDoor = new Door;

    returnDoor->doorNode1 = Door::baseDoorNode[doorType]->clone();
    returnDoor->doorNode2 = Door::baseDoorNode[doorType]->clone();

    unsigned char buttonIndex = 0;
    if (keycard>0) { buttonIndex=1; }
    if (code>0) { buttonIndex=2; }
    if (scanner>0) { buttonIndex=3; }
    returnDoor->buttonNode1 = Door::baseButtonNode[buttonIndex]->clone();
    returnDoor->buttonNode2 = Door::baseButtonNode[buttonIndex]->clone();

    returnDoor->frameNode = nullptr;
    if (doorType<2) {
        returnDoor->frameNode = Door::baseFrameNode->clone();
    }

    if (returnDoor->frameNode!=nullptr) {
        returnDoor->collider = Door::dynamics->addBoxObject(returnDoor->frameNode,10000.f);
        returnDoor->collider->setLinearFactor(btVector3(0.f,0.f,0.f));
        returnDoor->collider->setAngularFactor(btVector3(0.f,0.f,0.f));
    }

    returnDoor->frameNode->updateAbsolutePosition();

    return returnDoor;
}

void Door::setPosition(irr::core::vector3df newPos) {
    irr::core::matrix4 rot;
    rot.setRotationDegrees(frameNode->getRotation());
    irr::core::vector3df doorShiftVec(0.f,0.f,6.f*0.1f*RoomScale);
    irr::core::vector3df buttonShiftVec(-150.f*0.1f*RoomScale,0.f,24.f*0.1f*RoomScale);
    rot.transformVect(doorShiftVec);
    rot.transformVect(buttonShiftVec);

    doorNode1->setPosition(newPos+doorShiftVec);
    doorNode2->setPosition(newPos-doorShiftVec);
	frameNode->setPosition(newPos);
	buttonNode1->setPosition(newPos+buttonShiftVec+irr::core::vector3df(0.f,165.f*0.1f*RoomScale,0.f));
	buttonNode2->setPosition(newPos-buttonShiftVec+irr::core::vector3df(0.f,165.f*0.1f*RoomScale,0.f));
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

void Door::update() {

}

void Door::forceToggle() {

}

irr::core::vector3df Door::getPosition() {
    return frameNode->getAbsolutePosition();
}
