#include "ItemBat.h"

irr::scene::IMeshSceneNode* ItemBat::baseNode = nullptr;
btConvexHullShape* ItemBat::shape = nullptr;
irr::core::vector3df ItemBat::offset = irr::core::vector3df(0,0,0);

ItemBat::ItemBat() {}

void ItemBat::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemBat::baseNode == nullptr) {
        ItemBat::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemBat* ItemBat::createItemBat() {
    if (ItemBat::baseNode == nullptr) return nullptr;

    if (ItemBat::shape == nullptr) {
		Item::createShapeFromNode(ItemBat::baseNode,ItemBat::shape,ItemBat::offset);
	}

    ItemBat* newIt = new ItemBat;

    newIt->loadAssets(ItemBat::baseNode,ItemBat::shape);

    return newIt;
}

irr::core::vector3df ItemBat::getOffset() {
	return ItemBat::offset;
}

std::string ItemBat::getInvName() {
	return std::string("9V Battery");
}

std::string ItemBat::getInvImgPath() {
	return std::string("GFX/Items/INVbattery.jpg");
}

bool ItemBat::updateItem() {
	return false;
}

void ItemBat::updateWearing() {}
void ItemBat::drawItem() {}
