#include "itemKey6.h"

irr::scene::IMeshSceneNode* itemKey6::baseNode = nullptr;
btConvexHullShape* itemKey6::shape = nullptr;
irr::core::vector3df itemKey6::offset = irr::core::vector3df(0,0,0);

itemKey6::itemKey6() {}

void itemKey6::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemKey6::baseNode == nullptr) {
        itemKey6::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemKey6* itemKey6::createItemKey6() {
    if (itemKey6::baseNode == nullptr) return nullptr;

    if (itemKey6::shape == nullptr) {
		item::createShapeFromNode(itemKey6::baseNode,itemKey6::shape,itemKey6::offset);
	}

    itemKey6* newIt = new itemKey6;

    newIt->loadAssets(itemKey6::baseNode,itemKey6::shape);

    return newIt;
}

irr::core::vector3df itemKey6::getOffset() {
	return itemKey6::offset;
}

std::string itemKey6::getInvName() {
	return std::string("Key Card Omni");
}

bool itemKey6::updateItem() {
	return false;
}
