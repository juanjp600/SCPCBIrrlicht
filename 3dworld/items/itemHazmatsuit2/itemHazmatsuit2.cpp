#include "itemHazmatsuit2.h"

irr::scene::IMeshSceneNode* itemHazmatsuit2::baseNode = nullptr;
btConvexHullShape* itemHazmatsuit2::shape = nullptr;
irr::core::vector3df itemHazmatsuit2::offset = irr::core::vector3df(0,0,0);

itemHazmatsuit2::itemHazmatsuit2() {}

void itemHazmatsuit2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemHazmatsuit2::baseNode == nullptr) {
        itemHazmatsuit2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemHazmatsuit2* itemHazmatsuit2::createItemHazmatsuit2() {
    if (itemHazmatsuit2::baseNode == nullptr) return nullptr;

    if (itemHazmatsuit2::shape == nullptr) {
		item::createShapeFromNode(itemHazmatsuit2::baseNode,itemHazmatsuit2::shape,itemHazmatsuit2::offset);
	}

    itemHazmatsuit2* newIt = new itemHazmatsuit2;

    newIt->loadAssets(itemHazmatsuit2::baseNode,itemHazmatsuit2::shape);

    return newIt;
}

irr::core::vector3df itemHazmatsuit2::getOffset() {
	return itemHazmatsuit2::offset;
}

std::string itemHazmatsuit2::getInvName() {
	return std::string("Hazmat Suit");
}

bool itemHazmatsuit2::updateItem() {
	return false;
}
