#include "ItemFineeyedrops.h"

irr::scene::IMeshSceneNode* ItemFineeyedrops::baseNode = nullptr;
btConvexHullShape* ItemFineeyedrops::shape = nullptr;
irr::core::vector3df ItemFineeyedrops::offset = irr::core::vector3df(0,0,0);

ItemFineeyedrops::ItemFineeyedrops() {}

void ItemFineeyedrops::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemFineeyedrops::baseNode == nullptr) {
        ItemFineeyedrops::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemFineeyedrops* ItemFineeyedrops::createItemFineeyedrops() {
    if (ItemFineeyedrops::baseNode == nullptr) return nullptr;

    if (ItemFineeyedrops::shape == nullptr) {
		Item::createShapeFromNode(ItemFineeyedrops::baseNode,ItemFineeyedrops::shape,ItemFineeyedrops::offset);
	}

    ItemFineeyedrops* newIt = new ItemFineeyedrops;

    newIt->loadAssets(ItemFineeyedrops::baseNode,ItemFineeyedrops::shape);

    return newIt;
}

irr::core::vector3df ItemFineeyedrops::getOffset() {
	return ItemFineeyedrops::offset;
}

std::string ItemFineeyedrops::getInvName() {
	return std::string("Eyedrops");
}

std::string ItemFineeyedrops::getInvImgPath() {
	return std::string("GFX/Items/INVeyedrops.jpg");
}

bool ItemFineeyedrops::updateItem() {
	return false;
}

void ItemFineeyedrops::updateWearing() {}
void ItemFineeyedrops::drawItem() {}
void ItemFineeyedrops::setDrawCoords(irr::core::vector2di newCoords) {}
