#include "itemHand.h"

irr::scene::IMeshSceneNode* itemHand::baseNode = nullptr;
btConvexHullShape* itemHand::shape = nullptr;
irr::core::vector3df itemHand::offset = irr::core::vector3df(0,0,0);

itemHand::itemHand() {}

void itemHand::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemHand::baseNode == nullptr) {
        itemHand::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemHand* itemHand::createItemHand() {
    if (itemHand::baseNode == nullptr) return nullptr;

    if (itemHand::shape == nullptr) {
		item::createShapeFromNode(itemHand::baseNode,itemHand::shape,itemHand::offset);
	}

    itemHand* newIt = new itemHand;

    newIt->loadAssets(itemHand::baseNode,itemHand::shape);

    return newIt;
}

irr::core::vector3df itemHand::getOffset() {
	return itemHand::offset;
}

std::string itemHand::getInvName() {
	return std::string("Severed Hand");
}

bool itemHand::updateItem() {
	return false;
}
