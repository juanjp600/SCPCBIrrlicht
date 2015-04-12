#include "ItemCigarette.h"

irr::scene::IMeshSceneNode* ItemCigarette::baseNode = nullptr;
btConvexHullShape* ItemCigarette::shape = nullptr;
irr::core::vector3df ItemCigarette::offset = irr::core::vector3df(0,0,0);

ItemCigarette::ItemCigarette() {}

void ItemCigarette::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemCigarette::baseNode == nullptr) {
        ItemCigarette::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemCigarette* ItemCigarette::createItemCigarette() {
    if (ItemCigarette::baseNode == nullptr) return nullptr;

    if (ItemCigarette::shape == nullptr) {
		Item::createShapeFromNode(ItemCigarette::baseNode,ItemCigarette::shape,ItemCigarette::offset);
	}

    ItemCigarette* newIt = new ItemCigarette;

    newIt->loadAssets(ItemCigarette::baseNode,ItemCigarette::shape);

    return newIt;
}

irr::core::vector3df ItemCigarette::getOffset() {
	return ItemCigarette::offset;
}

std::string ItemCigarette::getInvName() {
	return std::string("Cigarette");
}

std::string ItemCigarette::getInvImgPath() {
	return std::string("GFX/Items/INV420.jpg");
}

bool ItemCigarette::updateItem() {
	return false;
}

void ItemCigarette::updateWearing() {}
void ItemCigarette::drawItem() {}
