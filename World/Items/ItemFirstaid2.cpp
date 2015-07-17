#include "ItemFirstaid2.h"

irr::scene::IMeshSceneNode* ItemFirstaid2::baseNode = nullptr;
btConvexHullShape* ItemFirstaid2::shape = nullptr;
irr::core::vector3df ItemFirstaid2::offset = irr::core::vector3df(0,0,0);

ItemFirstaid2::ItemFirstaid2() {}

void ItemFirstaid2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemFirstaid2::baseNode == nullptr) {
        ItemFirstaid2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemFirstaid2* ItemFirstaid2::createItemFirstaid2() {
    if (ItemFirstaid2::baseNode == nullptr) return nullptr;

    if (ItemFirstaid2::shape == nullptr) {
		Item::createShapeFromNode(ItemFirstaid2::baseNode,ItemFirstaid2::shape,ItemFirstaid2::offset);
	}

    ItemFirstaid2* newIt = new ItemFirstaid2;

    newIt->loadAssets(ItemFirstaid2::baseNode,ItemFirstaid2::shape);

    return newIt;
}

irr::core::vector3df ItemFirstaid2::getOffset() {
	return ItemFirstaid2::offset;
}

std::string ItemFirstaid2::getInvName() {
	return std::string("Blue First Aid Kit");
}

std::string ItemFirstaid2::getInvImgPath() {
	return std::string("GFX/Items/INVfirstaid2.jpg");
}

bool ItemFirstaid2::updateItem() {
	return false;
}

void ItemFirstaid2::updateWearing() {}
void ItemFirstaid2::drawItem() {}
void ItemFirstaid2::setDrawCoords(irr::core::vector2di newCoords) {}
