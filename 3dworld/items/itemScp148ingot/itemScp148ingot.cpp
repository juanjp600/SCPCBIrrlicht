#include "itemScp148ingot.h"

irr::scene::IMeshSceneNode* itemScp148ingot::baseNode = nullptr;
btConvexHullShape* itemScp148ingot::shape = nullptr;
irr::core::vector3df itemScp148ingot::offset = irr::core::vector3df(0,0,0);

itemScp148ingot::itemScp148ingot() {}

void itemScp148ingot::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemScp148ingot::baseNode == nullptr) {
        itemScp148ingot::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemScp148ingot* itemScp148ingot::createItemScp148ingot() {
    if (itemScp148ingot::baseNode == nullptr) return nullptr;

    if (itemScp148ingot::shape == nullptr) {
		item::createShapeFromNode(itemScp148ingot::baseNode,itemScp148ingot::shape,itemScp148ingot::offset);
	}

    itemScp148ingot* newIt = new itemScp148ingot;

    newIt->loadAssets(itemScp148ingot::baseNode,itemScp148ingot::shape);

    return newIt;
}

irr::core::vector3df itemScp148ingot::getOffset() {
	return itemScp148ingot::offset;
}

std::string itemScp148ingot::getInvName() {
	return std::string("SCP-148 Ingot");
}

std::string itemScp148ingot::getInvImgPath() {
	return std::string("GFX/items/INVscp148.jpg");
}

bool itemScp148ingot::updateItem() {
	return false;
}

void itemScp148ingot::updateWearing() {}
void itemScp148ingot::drawItem() {}
