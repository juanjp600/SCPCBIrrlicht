#include "ItemNav.h"

irr::scene::IMeshSceneNode* ItemNav::baseNode = nullptr;
btConvexHullShape* ItemNav::shape = nullptr;
irr::core::vector3df ItemNav::offset = irr::core::vector3df(0,0,0);

ItemNav::ItemNav() {}

void ItemNav::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemNav::baseNode == nullptr) {
        ItemNav::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemNav* ItemNav::createItemNav() {
    if (ItemNav::baseNode == nullptr) return nullptr;

    if (ItemNav::shape == nullptr) {
		Item::createShapeFromNode(ItemNav::baseNode,ItemNav::shape,ItemNav::offset);
	}

    ItemNav* newIt = new ItemNav;

    newIt->loadAssets(ItemNav::baseNode,ItemNav::shape);

    return newIt;
}

irr::core::vector3df ItemNav::getOffset() {
	return ItemNav::offset;
}

std::string ItemNav::getInvName() {
	return std::string("S-NAV 310 Navigator");
}

std::string ItemNav::getInvImgPath() {
	return std::string("GFX/Items/INVnavigator.jpg");
}

bool ItemNav::updateItem() {
	return false;
}

void ItemNav::updateWearing() {}
void ItemNav::drawItem() {}
