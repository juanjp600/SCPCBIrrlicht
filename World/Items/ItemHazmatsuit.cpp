#include "ItemHazmatsuit.h"

irr::scene::IMeshSceneNode* ItemHazmatsuit::baseNode = nullptr;
btConvexHullShape* ItemHazmatsuit::shape = nullptr;
irr::core::vector3df ItemHazmatsuit::offset = irr::core::vector3df(0,0,0);

ItemHazmatsuit::ItemHazmatsuit() {}

void ItemHazmatsuit::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemHazmatsuit::baseNode == nullptr) {
        ItemHazmatsuit::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemHazmatsuit* ItemHazmatsuit::createItemHazmatsuit() {
    if (ItemHazmatsuit::baseNode == nullptr) return nullptr;

    if (ItemHazmatsuit::shape == nullptr) {
		Item::createShapeFromNode(ItemHazmatsuit::baseNode,ItemHazmatsuit::shape,ItemHazmatsuit::offset);
	}

    ItemHazmatsuit* newIt = new ItemHazmatsuit;

    newIt->loadAssets(ItemHazmatsuit::baseNode,ItemHazmatsuit::shape);

    return newIt;
}

irr::core::vector3df ItemHazmatsuit::getOffset() {
	return ItemHazmatsuit::offset;
}

std::string ItemHazmatsuit::getInvName() {
	return std::string("Hazmat Suit");
}

std::string ItemHazmatsuit::getInvImgPath() {
	return std::string("GFX/Items/INVhazmat.jpg");
}

bool ItemHazmatsuit::updateItem() {
	return false;
}

void ItemHazmatsuit::updateWearing() {}
void ItemHazmatsuit::drawItem() {}
void ItemHazmatsuit::setDrawCoords(irr::core::vector2di newCoords) {}
