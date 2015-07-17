#include "ItemCup.h"

irr::scene::IMeshSceneNode* ItemCup::baseNode = nullptr;
btConvexHullShape* ItemCup::shape = nullptr;
irr::core::vector3df ItemCup::offset = irr::core::vector3df(0,0,0);

ItemCup::ItemCup() {}

void ItemCup::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemCup::baseNode == nullptr) {
        ItemCup::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemCup* ItemCup::createItemCup() {
    if (ItemCup::baseNode == nullptr) return nullptr;

    if (ItemCup::shape == nullptr) {
		Item::createShapeFromNode(ItemCup::baseNode,ItemCup::shape,ItemCup::offset);
	}

    ItemCup* newIt = new ItemCup;

    newIt->loadAssets(ItemCup::baseNode,ItemCup::shape);

    return newIt;
}

irr::core::vector3df ItemCup::getOffset() {
	return ItemCup::offset;
}

std::string ItemCup::getInvName() {
	return std::string("cup");
}

std::string ItemCup::getInvImgPath() {
	return std::string("GFX/Items/INVcup.jpg");
}

bool ItemCup::updateItem() {
	return false;
}

void ItemCup::updateWearing() {}
void ItemCup::drawItem() {}
void ItemCup::setDrawCoords(irr::core::vector2di newCoords) {}
