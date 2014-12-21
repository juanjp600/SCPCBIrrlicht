#include "item18vbat.h"

irr::scene::IMeshSceneNode* item18vbat::baseNode = nullptr;
btConvexHullShape* item18vbat::shape = nullptr;
irr::core::vector3df item18vbat::offset = irr::core::vector3df(0,0,0);

item18vbat::item18vbat() {}

void item18vbat::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (item18vbat::baseNode == nullptr) {
        item18vbat::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

item18vbat* item18vbat::createItem18vbat() {
    if (item18vbat::baseNode == nullptr) return nullptr;

    if (item18vbat::shape == nullptr) {
		item::createShapeFromNode(item18vbat::baseNode,item18vbat::shape,item18vbat::offset);
	}

    item18vbat* newIt = new item18vbat;

    newIt->loadAssets(item18vbat::baseNode,item18vbat::shape);

    return newIt;
}

irr::core::vector3df item18vbat::getOffset() {
	return item18vbat::offset;
}

std::string item18vbat::getInvName() {
	return std::string("18V Battery");
}

std::string item18vbat::getInvImgPath() {
	return std::string("GFX/items/INVbattery.jpg");
}

bool item18vbat::updateItem() {
	return false;
}

void item18vbat::updateWearing() {}
void item18vbat::drawItem() {}
