#include "ItemHazmatsuit2.h"

irr::scene::IMeshSceneNode* ItemHazmatsuit2::baseNode = nullptr;
btConvexHullShape* ItemHazmatsuit2::shape = nullptr;
irr::core::vector3df ItemHazmatsuit2::offset = irr::core::vector3df(0,0,0);

ItemHazmatsuit2::ItemHazmatsuit2() {}

void ItemHazmatsuit2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemHazmatsuit2::baseNode == nullptr) {
        ItemHazmatsuit2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemHazmatsuit2* ItemHazmatsuit2::createItemHazmatsuit2() {
    if (ItemHazmatsuit2::baseNode == nullptr) return nullptr;

    if (ItemHazmatsuit2::shape == nullptr) {
		Item::createShapeFromNode(ItemHazmatsuit2::baseNode,ItemHazmatsuit2::shape,ItemHazmatsuit2::offset);
	}

    ItemHazmatsuit2* newIt = new ItemHazmatsuit2;

    newIt->loadAssets(ItemHazmatsuit2::baseNode,ItemHazmatsuit2::shape);

    return newIt;
}

irr::core::vector3df ItemHazmatsuit2::getOffset() {
	return ItemHazmatsuit2::offset;
}

std::string ItemHazmatsuit2::getInvName() {
	return std::string("Hazmat Suit");
}

std::string ItemHazmatsuit2::getInvImgPath() {
	return std::string("GFX/Items/INVhazmat.jpg");
}

bool ItemHazmatsuit2::updateItem() {
	return false;
}

void ItemHazmatsuit2::updateWearing() {}
void ItemHazmatsuit2::drawItem() {}
