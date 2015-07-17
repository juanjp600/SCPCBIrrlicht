#include "ItemFinefirstaid.h"

irr::scene::IMeshSceneNode* ItemFinefirstaid::baseNode = nullptr;
btConvexHullShape* ItemFinefirstaid::shape = nullptr;
irr::core::vector3df ItemFinefirstaid::offset = irr::core::vector3df(0,0,0);

ItemFinefirstaid::ItemFinefirstaid() {}

void ItemFinefirstaid::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemFinefirstaid::baseNode == nullptr) {
        ItemFinefirstaid::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemFinefirstaid* ItemFinefirstaid::createItemFinefirstaid() {
    if (ItemFinefirstaid::baseNode == nullptr) return nullptr;

    if (ItemFinefirstaid::shape == nullptr) {
		Item::createShapeFromNode(ItemFinefirstaid::baseNode,ItemFinefirstaid::shape,ItemFinefirstaid::offset);
	}

    ItemFinefirstaid* newIt = new ItemFinefirstaid;

    newIt->loadAssets(ItemFinefirstaid::baseNode,ItemFinefirstaid::shape);

    return newIt;
}

irr::core::vector3df ItemFinefirstaid::getOffset() {
	return ItemFinefirstaid::offset;
}

std::string ItemFinefirstaid::getInvName() {
	return std::string("Small First Aid Kit");
}

std::string ItemFinefirstaid::getInvImgPath() {
	return std::string("GFX/Items/INVfirstaid.jpg");
}

bool ItemFinefirstaid::updateItem() {
	return false;
}

void ItemFinefirstaid::updateWearing() {}
void ItemFinefirstaid::drawItem() {}
void ItemFinefirstaid::setDrawCoords(irr::core::vector2di newCoords) {}
