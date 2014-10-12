#include "itemFinevest.h"

irr::scene::IMeshSceneNode* itemFinevest::baseNode = nullptr;
btConvexHullShape* itemFinevest::shape = nullptr;
irr::core::vector3df itemFinevest::offset = irr::core::vector3df(0,0,0);

itemFinevest::itemFinevest() {}

void itemFinevest::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemFinevest::baseNode == nullptr) {
        itemFinevest::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemFinevest* itemFinevest::createItemFinevest() {
    if (itemFinevest::baseNode == nullptr) return nullptr;

    if (itemFinevest::shape == nullptr) {
		item::createShapeFromNode(itemFinevest::baseNode,itemFinevest::shape,itemFinevest::offset);
	}

    itemFinevest* newIt = new itemFinevest;

    newIt->loadAssets(itemFinevest::baseNode,itemFinevest::shape);

    return newIt;
}

irr::core::vector3df itemFinevest::getOffset() {
	return itemFinevest::offset;
}

std::string itemFinevest::getInvName() {
	return std::string("Heavy Ballistic Vest");
}

bool itemFinevest::updateItem() {
	return false;
}
