#include "ItemHand2.h"

irr::scene::IMeshSceneNode* ItemHand2::baseNode = nullptr;
btConvexHullShape* ItemHand2::shape = nullptr;
irr::core::vector3df ItemHand2::offset = irr::core::vector3df(0,0,0);

ItemHand2::ItemHand2() {}

void ItemHand2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemHand2::baseNode == nullptr) {
        ItemHand2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemHand2* ItemHand2::createItemHand2() {
    if (ItemHand2::baseNode == nullptr) return nullptr;

    if (ItemHand2::shape == nullptr) {
		Item::createShapeFromNode(ItemHand2::baseNode,ItemHand2::shape,ItemHand2::offset);
	}

    ItemHand2* newIt = new ItemHand2;

    newIt->loadAssets(ItemHand2::baseNode,ItemHand2::shape);

    return newIt;
}

irr::core::vector3df ItemHand2::getOffset() {
	return ItemHand2::offset;
}

std::string ItemHand2::getInvName() {
	return std::string("Severed Hand");
}

std::string ItemHand2::getInvImgPath() {
	return std::string("GFX/Items/INVhand.jpg");
}

bool ItemHand2::updateItem() {
	return false;
}

void ItemHand2::updateWearing() {}
void ItemHand2::drawItem() {}
void ItemHand2::setDrawCoords(irr::core::vector2di newCoords) {}
