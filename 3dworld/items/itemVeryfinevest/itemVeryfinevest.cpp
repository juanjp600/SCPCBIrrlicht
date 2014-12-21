#include "itemVeryfinevest.h"

irr::scene::IMeshSceneNode* itemVeryfinevest::baseNode = nullptr;
btConvexHullShape* itemVeryfinevest::shape = nullptr;
irr::core::vector3df itemVeryfinevest::offset = irr::core::vector3df(0,0,0);

itemVeryfinevest::itemVeryfinevest() {}

void itemVeryfinevest::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemVeryfinevest::baseNode == nullptr) {
        itemVeryfinevest::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemVeryfinevest* itemVeryfinevest::createItemVeryfinevest() {
    if (itemVeryfinevest::baseNode == nullptr) return nullptr;

    if (itemVeryfinevest::shape == nullptr) {
		item::createShapeFromNode(itemVeryfinevest::baseNode,itemVeryfinevest::shape,itemVeryfinevest::offset);
	}

    itemVeryfinevest* newIt = new itemVeryfinevest;

    newIt->loadAssets(itemVeryfinevest::baseNode,itemVeryfinevest::shape);

    return newIt;
}

irr::core::vector3df itemVeryfinevest::getOffset() {
	return itemVeryfinevest::offset;
}

std::string itemVeryfinevest::getInvName() {
	return std::string("Bulky Ballistic Vest");
}

std::string itemVeryfinevest::getInvImgPath() {
	return std::string("GFX/items/INVvest.jpg");
}

bool itemVeryfinevest::updateItem() {
	return false;
}

void itemVeryfinevest::updateWearing() {}
void itemVeryfinevest::drawItem() {}
