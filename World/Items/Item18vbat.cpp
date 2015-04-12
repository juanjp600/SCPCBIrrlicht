#include "Item18vbat.h"

irr::scene::IMeshSceneNode* Item18vbat::baseNode = nullptr;
btConvexHullShape* Item18vbat::shape = nullptr;
irr::core::vector3df Item18vbat::offset = irr::core::vector3df(0,0,0);

Item18vbat::Item18vbat() {}

void Item18vbat::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (Item18vbat::baseNode == nullptr) {
        Item18vbat::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

Item18vbat* Item18vbat::createItem18vbat() {
    if (Item18vbat::baseNode == nullptr) return nullptr;

    if (Item18vbat::shape == nullptr) {
		Item::createShapeFromNode(Item18vbat::baseNode,Item18vbat::shape,Item18vbat::offset);
	}

    Item18vbat* newIt = new Item18vbat;

    newIt->loadAssets(Item18vbat::baseNode,Item18vbat::shape);

    return newIt;
}

irr::core::vector3df Item18vbat::getOffset() {
	return Item18vbat::offset;
}

std::string Item18vbat::getInvName() {
	return std::string("18V Battery");
}

std::string Item18vbat::getInvImgPath() {
	return std::string("GFX/Items/INVbattery.jpg");
}

bool Item18vbat::updateItem() {
	return false;
}

void Item18vbat::updateWearing() {}
void Item18vbat::drawItem() {}
