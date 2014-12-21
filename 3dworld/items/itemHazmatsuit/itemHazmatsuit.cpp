#include "itemHazmatsuit.h"

irr::scene::IMeshSceneNode* itemHazmatsuit::baseNode = nullptr;
btConvexHullShape* itemHazmatsuit::shape = nullptr;
irr::core::vector3df itemHazmatsuit::offset = irr::core::vector3df(0,0,0);

itemHazmatsuit::itemHazmatsuit() {}

void itemHazmatsuit::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemHazmatsuit::baseNode == nullptr) {
        itemHazmatsuit::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemHazmatsuit* itemHazmatsuit::createItemHazmatsuit() {
    if (itemHazmatsuit::baseNode == nullptr) return nullptr;

    if (itemHazmatsuit::shape == nullptr) {
		item::createShapeFromNode(itemHazmatsuit::baseNode,itemHazmatsuit::shape,itemHazmatsuit::offset);
	}

    itemHazmatsuit* newIt = new itemHazmatsuit;

    newIt->loadAssets(itemHazmatsuit::baseNode,itemHazmatsuit::shape);

    return newIt;
}

irr::core::vector3df itemHazmatsuit::getOffset() {
	return itemHazmatsuit::offset;
}

std::string itemHazmatsuit::getInvName() {
	return std::string("Hazmat Suit");
}

std::string itemHazmatsuit::getInvImgPath() {
	return std::string("GFX/items/INVhazmat.jpg");
}

bool itemHazmatsuit::updateItem() {
	return false;
}

void itemHazmatsuit::updateWearing() {}
void itemHazmatsuit::drawItem() {}
