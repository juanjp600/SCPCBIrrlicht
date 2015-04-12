#include "ItemScp1025.h"

irr::scene::IMeshSceneNode* ItemScp1025::baseNode = nullptr;
btConvexHullShape* ItemScp1025::shape = nullptr;
irr::core::vector3df ItemScp1025::offset = irr::core::vector3df(0,0,0);

ItemScp1025::ItemScp1025() {}

void ItemScp1025::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemScp1025::baseNode == nullptr) {
        ItemScp1025::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemScp1025* ItemScp1025::createItemScp1025() {
    if (ItemScp1025::baseNode == nullptr) return nullptr;

    if (ItemScp1025::shape == nullptr) {
		Item::createShapeFromNode(ItemScp1025::baseNode,ItemScp1025::shape,ItemScp1025::offset);
	}

    ItemScp1025* newIt = new ItemScp1025;

    newIt->loadAssets(ItemScp1025::baseNode,ItemScp1025::shape);

    return newIt;
}

irr::core::vector3df ItemScp1025::getOffset() {
	return ItemScp1025::offset;
}

std::string ItemScp1025::getInvName() {
	return std::string("SCP-1025");
}

std::string ItemScp1025::getInvImgPath() {
	return std::string("GFX/Items/INV1025.jpg");
}

bool ItemScp1025::updateItem() {
	return false;
}

void ItemScp1025::updateWearing() {}
void ItemScp1025::drawItem() {}
