#include "ItemScp513.h"

irr::scene::IMeshSceneNode* ItemScp513::baseNode = nullptr;
btConvexHullShape* ItemScp513::shape = nullptr;
irr::core::vector3df ItemScp513::offset = irr::core::vector3df(0,0,0);

ItemScp513::ItemScp513() {}

void ItemScp513::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemScp513::baseNode == nullptr) {
        ItemScp513::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemScp513* ItemScp513::createItemScp513() {
    if (ItemScp513::baseNode == nullptr) return nullptr;

    if (ItemScp513::shape == nullptr) {
		Item::createShapeFromNode(ItemScp513::baseNode,ItemScp513::shape,ItemScp513::offset);
	}

    ItemScp513* newIt = new ItemScp513;

    newIt->loadAssets(ItemScp513::baseNode,ItemScp513::shape);

    return newIt;
}

irr::core::vector3df ItemScp513::getOffset() {
	return ItemScp513::offset;
}

std::string ItemScp513::getInvName() {
	return std::string("SCP-513");
}

std::string ItemScp513::getInvImgPath() {
	return std::string("GFX/Items/INV513.jpg");
}

bool ItemScp513::updateItem() {
	return false;
}

void ItemScp513::updateWearing() {}
void ItemScp513::drawItem() {}
