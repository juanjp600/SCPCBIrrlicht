#include "ItemMisc.h"

irr::scene::IMeshSceneNode* ItemMisc::baseNode = nullptr;
btConvexHullShape* ItemMisc::shape = nullptr;
irr::core::vector3df ItemMisc::offset = irr::core::vector3df(0,0,0);

ItemMisc::ItemMisc() {}

void ItemMisc::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemMisc::baseNode == nullptr) {
        ItemMisc::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemMisc* ItemMisc::createItemMisc() {
    if (ItemMisc::baseNode == nullptr) return nullptr;

    if (ItemMisc::shape == nullptr) {
		Item::createShapeFromNode(ItemMisc::baseNode,ItemMisc::shape,ItemMisc::offset);
	}

    ItemMisc* newIt = new ItemMisc;

    newIt->loadAssets(ItemMisc::baseNode,ItemMisc::shape);

    return newIt;
}

irr::core::vector3df ItemMisc::getOffset() {
	return ItemMisc::offset;
}

std::string ItemMisc::getInvName() {
	return std::string("Electronical components");
}

std::string ItemMisc::getInvImgPath() {
	return std::string("GFX/Items/INVelectronics.jpg");
}

bool ItemMisc::updateItem() {
	return false;
}

void ItemMisc::updateWearing() {}
void ItemMisc::drawItem() {}
