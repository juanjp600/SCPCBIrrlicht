#include "ItemScp148ingot.h"

irr::scene::IMeshSceneNode* ItemScp148ingot::baseNode = nullptr;
btConvexHullShape* ItemScp148ingot::shape = nullptr;
irr::core::vector3df ItemScp148ingot::offset = irr::core::vector3df(0,0,0);

ItemScp148ingot::ItemScp148ingot() {}

void ItemScp148ingot::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemScp148ingot::baseNode == nullptr) {
        ItemScp148ingot::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemScp148ingot* ItemScp148ingot::createItemScp148ingot() {
    if (ItemScp148ingot::baseNode == nullptr) return nullptr;

    if (ItemScp148ingot::shape == nullptr) {
		Item::createShapeFromNode(ItemScp148ingot::baseNode,ItemScp148ingot::shape,ItemScp148ingot::offset);
	}

    ItemScp148ingot* newIt = new ItemScp148ingot;

    newIt->loadAssets(ItemScp148ingot::baseNode,ItemScp148ingot::shape);

    return newIt;
}

irr::core::vector3df ItemScp148ingot::getOffset() {
	return ItemScp148ingot::offset;
}

std::string ItemScp148ingot::getInvName() {
	return std::string("SCP-148 Ingot");
}

std::string ItemScp148ingot::getInvImgPath() {
	return std::string("GFX/Items/INVscp148.jpg");
}

bool ItemScp148ingot::updateItem() {
	return false;
}

void ItemScp148ingot::updateWearing() {}
void ItemScp148ingot::drawItem() {}
void ItemScp148ingot::setDrawCoords(irr::core::vector2di newCoords) {}
