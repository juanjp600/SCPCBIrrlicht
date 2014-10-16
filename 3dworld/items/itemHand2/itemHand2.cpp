#include "itemHand2.h"

irr::scene::IMeshSceneNode* itemHand2::baseNode = nullptr;
btConvexHullShape* itemHand2::shape = nullptr;
irr::core::vector3df itemHand2::offset = irr::core::vector3df(0,0,0);

itemHand2::itemHand2() {}

void itemHand2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemHand2::baseNode == nullptr) {
        itemHand2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemHand2* itemHand2::createItemHand2() {
    if (itemHand2::baseNode == nullptr) return nullptr;

    if (itemHand2::shape == nullptr) {
		item::createShapeFromNode(itemHand2::baseNode,itemHand2::shape,itemHand2::offset);
	}

    itemHand2* newIt = new itemHand2;

    newIt->loadAssets(itemHand2::baseNode,itemHand2::shape);

    return newIt;
}

irr::core::vector3df itemHand2::getOffset() {
	return itemHand2::offset;
}

std::string itemHand2::getInvName() {
	return std::string("Severed Hand");
}

std::string itemHand2::getInvImgPath() {
	return std::string("GFX/items/INVhand.jpg");
}

bool itemHand2::updateItem() {
	return false;
}
