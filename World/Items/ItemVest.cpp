#include "ItemVest.h"

irr::scene::IMeshSceneNode* ItemVest::baseNode = nullptr;
btConvexHullShape* ItemVest::shape = nullptr;
irr::core::vector3df ItemVest::offset = irr::core::vector3df(0,0,0);

ItemVest::ItemVest() {}

void ItemVest::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemVest::baseNode == nullptr) {
        ItemVest::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemVest* ItemVest::createItemVest() {
    if (ItemVest::baseNode == nullptr) return nullptr;

    if (ItemVest::shape == nullptr) {
		Item::createShapeFromNode(ItemVest::baseNode,ItemVest::shape,ItemVest::offset);
	}

    ItemVest* newIt = new ItemVest;

    newIt->loadAssets(ItemVest::baseNode,ItemVest::shape);

    return newIt;
}

irr::core::vector3df ItemVest::getOffset() {
	return ItemVest::offset;
}

std::string ItemVest::getInvName() {
	return std::string("Ballistic Vest");
}

std::string ItemVest::getInvImgPath() {
	return std::string("GFX/Items/INVvest.jpg");
}

bool ItemVest::updateItem() {
	return false;
}

void ItemVest::updateWearing() {}
void ItemVest::drawItem() {}
