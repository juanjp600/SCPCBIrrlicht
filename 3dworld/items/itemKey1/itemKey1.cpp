#include "itemKey1.h"

irr::scene::IMeshSceneNode* itemKey1::baseNode = nullptr;
btConvexHullShape* itemKey1::shape = nullptr;
irr::core::vector3df itemKey1::offset = irr::core::vector3df(0,0,0);

itemKey1::itemKey1() {}

void itemKey1::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemKey1::baseNode == nullptr) {
        itemKey1::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemKey1* itemKey1::createItemKey1() {
    if (itemKey1::baseNode == nullptr) return nullptr;

    if (itemKey1::shape == nullptr) {
		item::createShapeFromNode(itemKey1::baseNode,itemKey1::shape,itemKey1::offset);
	}

    itemKey1* newIt = new itemKey1;

    newIt->loadAssets(itemKey1::baseNode,itemKey1::shape);

    return newIt;
}

irr::core::vector3df itemKey1::getOffset() {
	return itemKey1::offset;
}

std::string itemKey1::getInvName() {
	return std::string("Level 1 Key Card");
}

bool itemKey1::updateItem() {
	return false;
}
