#include "itemKey5.h"

irr::scene::IMeshSceneNode* itemKey5::baseNode = nullptr;
btConvexHullShape* itemKey5::shape = nullptr;
irr::core::vector3df itemKey5::offset = irr::core::vector3df(0,0,0);

itemKey5::itemKey5() {}

void itemKey5::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemKey5::baseNode == nullptr) {
        itemKey5::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemKey5* itemKey5::createItemKey5() {
    if (itemKey5::baseNode == nullptr) return nullptr;

    if (itemKey5::shape == nullptr) {
		item::createShapeFromNode(itemKey5::baseNode,itemKey5::shape,itemKey5::offset);
	}

    itemKey5* newIt = new itemKey5;

    newIt->loadAssets(itemKey5::baseNode,itemKey5::shape);

    return newIt;
}

irr::core::vector3df itemKey5::getOffset() {
	return itemKey5::offset;
}

std::string itemKey5::getInvName() {
	return std::string("Level 5 Key Card");
}

bool itemKey5::updateItem() {
	return false;
}
