#include "ItemScp148.h"

irr::scene::IMeshSceneNode* ItemScp148::baseNode = nullptr;
btConvexHullShape* ItemScp148::shape = nullptr;
irr::core::vector3df ItemScp148::offset = irr::core::vector3df(0,0,0);

ItemScp148::ItemScp148() {}

void ItemScp148::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemScp148::baseNode == nullptr) {
        ItemScp148::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemScp148* ItemScp148::createItemScp148() {
    if (ItemScp148::baseNode == nullptr) return nullptr;

    if (ItemScp148::shape == nullptr) {
		Item::createShapeFromNode(ItemScp148::baseNode,ItemScp148::shape,ItemScp148::offset);
	}

    ItemScp148* newIt = new ItemScp148;

    newIt->loadAssets(ItemScp148::baseNode,ItemScp148::shape);

    return newIt;
}

irr::core::vector3df ItemScp148::getOffset() {
	return ItemScp148::offset;
}

std::string ItemScp148::getInvName() {
	return std::string("Metal Panel");
}

std::string ItemScp148::getInvImgPath() {
	return std::string("GFX/Items/INVmetalpanel.jpg");
}

bool ItemScp148::updateItem() {
	return false;
}

void ItemScp148::updateWearing() {}
void ItemScp148::drawItem() {}
void ItemScp148::setDrawCoords(irr::core::vector2di newCoords) {}
