#include "itemCup.h"

irr::scene::IMeshSceneNode* itemCup::baseNode = nullptr;
btConvexHullShape* itemCup::shape = nullptr;
irr::core::vector3df itemCup::offset = irr::core::vector3df(0,0,0);

itemCup::itemCup() {}

void itemCup::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemCup::baseNode == nullptr) {
        itemCup::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemCup* itemCup::createItemCup() {
    if (itemCup::baseNode == nullptr) return nullptr;

    if (itemCup::shape == nullptr) {
		item::createShapeFromNode(itemCup::baseNode,itemCup::shape,itemCup::offset);
	}

    itemCup* newIt = new itemCup;

    newIt->loadAssets(itemCup::baseNode,itemCup::shape);

    return newIt;
}

irr::core::vector3df itemCup::getOffset() {
	return itemCup::offset;
}

std::string itemCup::getInvName() {
	return std::string("cup");
}

std::string itemCup::getInvImgPath() {
	return std::string("GFX/items/INVcup.jpg");
}

bool itemCup::updateItem() {
	return false;
}
