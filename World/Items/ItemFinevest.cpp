#include "ItemFinevest.h"

irr::scene::IMeshSceneNode* ItemFinevest::baseNode = nullptr;
btConvexHullShape* ItemFinevest::shape = nullptr;
irr::core::vector3df ItemFinevest::offset = irr::core::vector3df(0,0,0);

ItemFinevest::ItemFinevest() {}

void ItemFinevest::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemFinevest::baseNode == nullptr) {
        ItemFinevest::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemFinevest* ItemFinevest::createItemFinevest() {
    if (ItemFinevest::baseNode == nullptr) return nullptr;

    if (ItemFinevest::shape == nullptr) {
		Item::createShapeFromNode(ItemFinevest::baseNode,ItemFinevest::shape,ItemFinevest::offset);
	}

    ItemFinevest* newIt = new ItemFinevest;

    newIt->loadAssets(ItemFinevest::baseNode,ItemFinevest::shape);

    return newIt;
}

irr::core::vector3df ItemFinevest::getOffset() {
	return ItemFinevest::offset;
}

std::string ItemFinevest::getInvName() {
	return std::string("Heavy Ballistic Vest");
}

std::string ItemFinevest::getInvImgPath() {
	return std::string("GFX/Items/INVvest.jpg");
}

bool ItemFinevest::updateItem() {
	return false;
}

void ItemFinevest::updateWearing() {}
void ItemFinevest::drawItem() {}
void ItemFinevest::setDrawCoords(irr::core::vector2di newCoords) {}
