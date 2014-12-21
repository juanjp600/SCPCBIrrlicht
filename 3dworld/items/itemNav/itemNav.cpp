#include "itemNav.h"

irr::scene::IMeshSceneNode* itemNav::baseNode = nullptr;
btConvexHullShape* itemNav::shape = nullptr;
irr::core::vector3df itemNav::offset = irr::core::vector3df(0,0,0);

itemNav::itemNav() {}

void itemNav::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemNav::baseNode == nullptr) {
        itemNav::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemNav* itemNav::createItemNav() {
    if (itemNav::baseNode == nullptr) return nullptr;

    if (itemNav::shape == nullptr) {
		item::createShapeFromNode(itemNav::baseNode,itemNav::shape,itemNav::offset);
	}

    itemNav* newIt = new itemNav;

    newIt->loadAssets(itemNav::baseNode,itemNav::shape);

    return newIt;
}

irr::core::vector3df itemNav::getOffset() {
	return itemNav::offset;
}

std::string itemNav::getInvName() {
	return std::string("S-NAV 310 Navigator");
}

std::string itemNav::getInvImgPath() {
	return std::string("GFX/items/INVnavigator.jpg");
}

bool itemNav::updateItem() {
	return false;
}

void itemNav::updateWearing() {}
void itemNav::drawItem() {}
