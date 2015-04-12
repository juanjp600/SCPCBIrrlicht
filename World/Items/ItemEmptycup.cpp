#include "ItemEmptycup.h"

irr::scene::IMeshSceneNode* ItemEmptycup::baseNode = nullptr;
btConvexHullShape* ItemEmptycup::shape = nullptr;
irr::core::vector3df ItemEmptycup::offset = irr::core::vector3df(0,0,0);

ItemEmptycup::ItemEmptycup() {}

void ItemEmptycup::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemEmptycup::baseNode == nullptr) {
        ItemEmptycup::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemEmptycup* ItemEmptycup::createItemEmptycup() {
    if (ItemEmptycup::baseNode == nullptr) return nullptr;

    if (ItemEmptycup::shape == nullptr) {
		Item::createShapeFromNode(ItemEmptycup::baseNode,ItemEmptycup::shape,ItemEmptycup::offset);
	}

    ItemEmptycup* newIt = new ItemEmptycup;

    newIt->loadAssets(ItemEmptycup::baseNode,ItemEmptycup::shape);

    return newIt;
}

irr::core::vector3df ItemEmptycup::getOffset() {
	return ItemEmptycup::offset;
}

std::string ItemEmptycup::getInvName() {
	return std::string("Empty Cup");
}

std::string ItemEmptycup::getInvImgPath() {
	return std::string("GFX/Items/INVcup.jpg");
}

bool ItemEmptycup::updateItem() {
	return false;
}

void ItemEmptycup::updateWearing() {}
void ItemEmptycup::drawItem() {}
