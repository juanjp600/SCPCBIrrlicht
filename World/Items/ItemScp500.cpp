#include "ItemScp500.h"

irr::scene::IMeshSceneNode* ItemScp500::baseNode = nullptr;
btConvexHullShape* ItemScp500::shape = nullptr;
irr::core::vector3df ItemScp500::offset = irr::core::vector3df(0,0,0);

ItemScp500::ItemScp500() {}

void ItemScp500::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemScp500::baseNode == nullptr) {
        ItemScp500::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemScp500* ItemScp500::createItemScp500() {
    if (ItemScp500::baseNode == nullptr) return nullptr;

    if (ItemScp500::shape == nullptr) {
		Item::createShapeFromNode(ItemScp500::baseNode,ItemScp500::shape,ItemScp500::offset);
	}

    ItemScp500* newIt = new ItemScp500;

    newIt->loadAssets(ItemScp500::baseNode,ItemScp500::shape);

    return newIt;
}

irr::core::vector3df ItemScp500::getOffset() {
	return ItemScp500::offset;
}

std::string ItemScp500::getInvName() {
	return std::string("SCP-500-01");
}

std::string ItemScp500::getInvImgPath() {
	return std::string("GFX/Items/INVpill.jpg");
}

bool ItemScp500::updateItem() {
	return false;
}

void ItemScp500::updateWearing() {}
void ItemScp500::drawItem() {}
void ItemScp500::setDrawCoords(irr::core::vector2di newCoords) {}
