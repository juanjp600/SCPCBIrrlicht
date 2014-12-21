#include "itemHazmatsuit3.h"

irr::scene::IMeshSceneNode* itemHazmatsuit3::baseNode = nullptr;
btConvexHullShape* itemHazmatsuit3::shape = nullptr;
irr::core::vector3df itemHazmatsuit3::offset = irr::core::vector3df(0,0,0);

itemHazmatsuit3::itemHazmatsuit3() {}

void itemHazmatsuit3::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemHazmatsuit3::baseNode == nullptr) {
        itemHazmatsuit3::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemHazmatsuit3* itemHazmatsuit3::createItemHazmatsuit3() {
    if (itemHazmatsuit3::baseNode == nullptr) return nullptr;

    if (itemHazmatsuit3::shape == nullptr) {
		item::createShapeFromNode(itemHazmatsuit3::baseNode,itemHazmatsuit3::shape,itemHazmatsuit3::offset);
	}

    itemHazmatsuit3* newIt = new itemHazmatsuit3;

    newIt->loadAssets(itemHazmatsuit3::baseNode,itemHazmatsuit3::shape);

    return newIt;
}

irr::core::vector3df itemHazmatsuit3::getOffset() {
	return itemHazmatsuit3::offset;
}

std::string itemHazmatsuit3::getInvName() {
	return std::string("Heavy Hazmat Suit");
}

std::string itemHazmatsuit3::getInvImgPath() {
	return std::string("GFX/items/INVhazmat.jpg");
}

bool itemHazmatsuit3::updateItem() {
	return false;
}

void itemHazmatsuit3::updateWearing() {}
void itemHazmatsuit3::drawItem() {}
