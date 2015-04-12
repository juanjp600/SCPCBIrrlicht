#include "ItemSupereyedrops.h"

irr::scene::IMeshSceneNode* ItemSupereyedrops::baseNode = nullptr;
btConvexHullShape* ItemSupereyedrops::shape = nullptr;
irr::core::vector3df ItemSupereyedrops::offset = irr::core::vector3df(0,0,0);

ItemSupereyedrops::ItemSupereyedrops() {}

void ItemSupereyedrops::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemSupereyedrops::baseNode == nullptr) {
        ItemSupereyedrops::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemSupereyedrops* ItemSupereyedrops::createItemSupereyedrops() {
    if (ItemSupereyedrops::baseNode == nullptr) return nullptr;

    if (ItemSupereyedrops::shape == nullptr) {
		Item::createShapeFromNode(ItemSupereyedrops::baseNode,ItemSupereyedrops::shape,ItemSupereyedrops::offset);
	}

    ItemSupereyedrops* newIt = new ItemSupereyedrops;

    newIt->loadAssets(ItemSupereyedrops::baseNode,ItemSupereyedrops::shape);

    return newIt;
}

irr::core::vector3df ItemSupereyedrops::getOffset() {
	return ItemSupereyedrops::offset;
}

std::string ItemSupereyedrops::getInvName() {
	return std::string("Eyedrops");
}

std::string ItemSupereyedrops::getInvImgPath() {
	return std::string("GFX/Items/INVeyedrops.jpg");
}

bool ItemSupereyedrops::updateItem() {
	return false;
}

void ItemSupereyedrops::updateWearing() {}
void ItemSupereyedrops::drawItem() {}
