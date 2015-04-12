#include "ItemVeryfinevest.h"

irr::scene::IMeshSceneNode* ItemVeryfinevest::baseNode = nullptr;
btConvexHullShape* ItemVeryfinevest::shape = nullptr;
irr::core::vector3df ItemVeryfinevest::offset = irr::core::vector3df(0,0,0);

ItemVeryfinevest::ItemVeryfinevest() {}

void ItemVeryfinevest::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemVeryfinevest::baseNode == nullptr) {
        ItemVeryfinevest::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemVeryfinevest* ItemVeryfinevest::createItemVeryfinevest() {
    if (ItemVeryfinevest::baseNode == nullptr) return nullptr;

    if (ItemVeryfinevest::shape == nullptr) {
		Item::createShapeFromNode(ItemVeryfinevest::baseNode,ItemVeryfinevest::shape,ItemVeryfinevest::offset);
	}

    ItemVeryfinevest* newIt = new ItemVeryfinevest;

    newIt->loadAssets(ItemVeryfinevest::baseNode,ItemVeryfinevest::shape);

    return newIt;
}

irr::core::vector3df ItemVeryfinevest::getOffset() {
	return ItemVeryfinevest::offset;
}

std::string ItemVeryfinevest::getInvName() {
	return std::string("Bulky Ballistic Vest");
}

std::string ItemVeryfinevest::getInvImgPath() {
	return std::string("GFX/Items/INVvest.jpg");
}

bool ItemVeryfinevest::updateItem() {
	return false;
}

void ItemVeryfinevest::updateWearing() {}
void ItemVeryfinevest::drawItem() {}
