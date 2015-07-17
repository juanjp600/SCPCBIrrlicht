#include "ItemHand.h"

irr::scene::IMeshSceneNode* ItemHand::baseNode = nullptr;
btConvexHullShape* ItemHand::shape = nullptr;
irr::core::vector3df ItemHand::offset = irr::core::vector3df(0,0,0);

ItemHand::ItemHand() {}

void ItemHand::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemHand::baseNode == nullptr) {
        ItemHand::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemHand* ItemHand::createItemHand() {
    if (ItemHand::baseNode == nullptr) return nullptr;

    if (ItemHand::shape == nullptr) {
		Item::createShapeFromNode(ItemHand::baseNode,ItemHand::shape,ItemHand::offset);
	}

    ItemHand* newIt = new ItemHand;

    newIt->loadAssets(ItemHand::baseNode,ItemHand::shape);

    return newIt;
}

irr::core::vector3df ItemHand::getOffset() {
	return ItemHand::offset;
}

std::string ItemHand::getInvName() {
	return std::string("Severed Hand");
}

std::string ItemHand::getInvImgPath() {
	return std::string("GFX/Items/INVhand.jpg");
}

bool ItemHand::updateItem() {
	return false;
}

void ItemHand::updateWearing() {}
void ItemHand::drawItem() {}
void ItemHand::setDrawCoords(irr::core::vector2di newCoords) {}
