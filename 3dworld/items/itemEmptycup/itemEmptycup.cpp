#include "itemEmptycup.h"

irr::scene::IMeshSceneNode* itemEmptycup::baseNode = nullptr;
btConvexHullShape* itemEmptycup::shape = nullptr;
irr::core::vector3df itemEmptycup::offset = irr::core::vector3df(0,0,0);

itemEmptycup::itemEmptycup() {}

void itemEmptycup::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemEmptycup::baseNode == nullptr) {
        itemEmptycup::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemEmptycup* itemEmptycup::createItemEmptycup() {
    if (itemEmptycup::baseNode == nullptr) return nullptr;

    if (itemEmptycup::shape == nullptr) {
		item::createShapeFromNode(itemEmptycup::baseNode,itemEmptycup::shape,itemEmptycup::offset);
	}

    itemEmptycup* newIt = new itemEmptycup;

    newIt->loadAssets(itemEmptycup::baseNode,itemEmptycup::shape);

    return newIt;
}

irr::core::vector3df itemEmptycup::getOffset() {
	return itemEmptycup::offset;
}

std::string itemEmptycup::getInvName() {
	return std::string("Empty Cup");
}

bool itemEmptycup::updateItem() {
	return false;
}
