#include "itemBat.h"

irr::scene::IMeshSceneNode* itemBat::baseNode = nullptr;
btConvexHullShape* itemBat::shape = nullptr;
irr::core::vector3df itemBat::offset = irr::core::vector3df(0,0,0);

itemBat::itemBat() {}

void itemBat::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemBat::baseNode == nullptr) {
        itemBat::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemBat* itemBat::createItemBat() {
    if (itemBat::baseNode == nullptr) return nullptr;

    if (itemBat::shape == nullptr) {
		item::createShapeFromNode(itemBat::baseNode,itemBat::shape,itemBat::offset);
	}

    itemBat* newIt = new itemBat;

    newIt->loadAssets(itemBat::baseNode,itemBat::shape);

    return newIt;
}

irr::core::vector3df itemBat::getOffset() {
	return itemBat::offset;
}

std::string itemBat::getInvName() {
	return std::string("9V Battery");
}

std::string itemBat::getInvImgPath() {
	return std::string("GFX/items/INVbattery.jpg");
}

bool itemBat::updateItem() {
	return false;
}
