#include "itemKey2.h"

irr::scene::IMeshSceneNode* itemKey2::baseNode = nullptr;
btConvexHullShape* itemKey2::shape = nullptr;
irr::core::vector3df itemKey2::offset = irr::core::vector3df(0,0,0);

itemKey2::itemKey2() {}

void itemKey2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemKey2::baseNode == nullptr) {
        itemKey2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemKey2* itemKey2::createItemKey2() {
    if (itemKey2::baseNode == nullptr) return nullptr;

    if (itemKey2::shape == nullptr) {
		item::createShapeFromNode(itemKey2::baseNode,itemKey2::shape,itemKey2::offset);
	}

    itemKey2* newIt = new itemKey2;

    newIt->loadAssets(itemKey2::baseNode,itemKey2::shape);

    return newIt;
}

irr::core::vector3df itemKey2::getOffset() {
	return itemKey2::offset;
}

std::string itemKey2::getInvName() {
	return std::string("Level 2 Key Card");
}

bool itemKey2::updateItem() {
	return false;
}
