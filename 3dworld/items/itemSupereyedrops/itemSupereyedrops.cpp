#include "itemSupereyedrops.h"

irr::scene::IMeshSceneNode* itemSupereyedrops::baseNode = nullptr;
btConvexHullShape* itemSupereyedrops::shape = nullptr;
irr::core::vector3df itemSupereyedrops::offset = irr::core::vector3df(0,0,0);

itemSupereyedrops::itemSupereyedrops() {}

void itemSupereyedrops::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemSupereyedrops::baseNode == nullptr) {
        itemSupereyedrops::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemSupereyedrops* itemSupereyedrops::createItemSupereyedrops() {
    if (itemSupereyedrops::baseNode == nullptr) return nullptr;

    if (itemSupereyedrops::shape == nullptr) {
		item::createShapeFromNode(itemSupereyedrops::baseNode,itemSupereyedrops::shape,itemSupereyedrops::offset);
	}

    itemSupereyedrops* newIt = new itemSupereyedrops;

    newIt->loadAssets(itemSupereyedrops::baseNode,itemSupereyedrops::shape);

    return newIt;
}

irr::core::vector3df itemSupereyedrops::getOffset() {
	return itemSupereyedrops::offset;
}

std::string itemSupereyedrops::getInvName() {
	return std::string("Eyedrops");
}

std::string itemSupereyedrops::getInvImgPath() {
	return std::string("GFX/items/INVeyedrops.jpg");
}

bool itemSupereyedrops::updateItem() {
	return false;
}

void itemSupereyedrops::updateWearing() {}
void itemSupereyedrops::drawItem() {}
