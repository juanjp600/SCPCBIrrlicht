#include "ItemFirstaid.h"

irr::scene::IMeshSceneNode* ItemFirstaid::baseNode = nullptr;
btConvexHullShape* ItemFirstaid::shape = nullptr;
irr::core::vector3df ItemFirstaid::offset = irr::core::vector3df(0,0,0);

ItemFirstaid::ItemFirstaid() {}

void ItemFirstaid::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemFirstaid::baseNode == nullptr) {
        ItemFirstaid::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemFirstaid* ItemFirstaid::createItemFirstaid() {
    if (ItemFirstaid::baseNode == nullptr) return nullptr;

    if (ItemFirstaid::shape == nullptr) {
		Item::createShapeFromNode(ItemFirstaid::baseNode,ItemFirstaid::shape,ItemFirstaid::offset);
	}

    ItemFirstaid* newIt = new ItemFirstaid;

    newIt->loadAssets(ItemFirstaid::baseNode,ItemFirstaid::shape);

    return newIt;
}

irr::core::vector3df ItemFirstaid::getOffset() {
	return ItemFirstaid::offset;
}

std::string ItemFirstaid::getInvName() {
	return std::string("First Aid Kit");
}

std::string ItemFirstaid::getInvImgPath() {
	return std::string("GFX/Items/INVfirstaid.jpg");
}

bool ItemFirstaid::updateItem() {
	return false;
}

void ItemFirstaid::updateWearing() {}
void ItemFirstaid::drawItem() {}
void ItemFirstaid::setDrawCoords(irr::core::vector2di newCoords) {}
