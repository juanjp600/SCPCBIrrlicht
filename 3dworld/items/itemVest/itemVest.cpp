#include "itemVest.h"

irr::scene::IMeshSceneNode* itemVest::baseNode = nullptr;
btConvexHullShape* itemVest::shape = nullptr;
irr::core::vector3df itemVest::offset = irr::core::vector3df(0,0,0);

itemVest::itemVest() {}

void itemVest::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemVest::baseNode == nullptr) {
        itemVest::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemVest* itemVest::createItemVest() {
    if (itemVest::baseNode == nullptr) return nullptr;

    if (itemVest::shape == nullptr) {
		item::createShapeFromNode(itemVest::baseNode,itemVest::shape,itemVest::offset);
	}

    itemVest* newIt = new itemVest;

    newIt->loadAssets(itemVest::baseNode,itemVest::shape);

    return newIt;
}

irr::core::vector3df itemVest::getOffset() {
	return itemVest::offset;
}

std::string itemVest::getInvName() {
	return std::string("Ballistic Vest");
}

std::string itemVest::getInvImgPath() {
	return std::string("GFX/items/INVvest.jpg");
}

bool itemVest::updateItem() {
	return false;
}
