#include "ItemEyedrops.h"

irr::scene::IMeshSceneNode* ItemEyedrops::baseNode = nullptr;
btConvexHullShape* ItemEyedrops::shape = nullptr;
irr::core::vector3df ItemEyedrops::offset = irr::core::vector3df(0,0,0);

ItemEyedrops::ItemEyedrops() {}

void ItemEyedrops::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemEyedrops::baseNode == nullptr) {
        ItemEyedrops::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemEyedrops* ItemEyedrops::createItemEyedrops() {
    if (ItemEyedrops::baseNode == nullptr) return nullptr;

    if (ItemEyedrops::shape == nullptr) {
		Item::createShapeFromNode(ItemEyedrops::baseNode,ItemEyedrops::shape,ItemEyedrops::offset);
	}

    ItemEyedrops* newIt = new ItemEyedrops;

    newIt->loadAssets(ItemEyedrops::baseNode,ItemEyedrops::shape);

    return newIt;
}

irr::core::vector3df ItemEyedrops::getOffset() {
	return ItemEyedrops::offset;
}

std::string ItemEyedrops::getInvName() {
	return std::string("RedVision Eyedrops");
}

std::string ItemEyedrops::getInvImgPath() {
	return std::string("GFX/Items/INVeyedropsred.jpg");
}

bool ItemEyedrops::updateItem() {
	return false;
}

void ItemEyedrops::updateWearing() {}
void ItemEyedrops::drawItem() {}
