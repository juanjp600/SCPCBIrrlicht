#include "ItemScp860.h"

irr::scene::IMeshSceneNode* ItemScp860::baseNode = nullptr;
btConvexHullShape* ItemScp860::shape = nullptr;
irr::core::vector3df ItemScp860::offset = irr::core::vector3df(0,0,0);

ItemScp860::ItemScp860() {}

void ItemScp860::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemScp860::baseNode == nullptr) {
        ItemScp860::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemScp860* ItemScp860::createItemScp860() {
    if (ItemScp860::baseNode == nullptr) return nullptr;

    if (ItemScp860::shape == nullptr) {
		Item::createShapeFromNode(ItemScp860::baseNode,ItemScp860::shape,ItemScp860::offset);
	}

    ItemScp860* newIt = new ItemScp860;

    newIt->loadAssets(ItemScp860::baseNode,ItemScp860::shape);

    return newIt;
}

irr::core::vector3df ItemScp860::getOffset() {
	return ItemScp860::offset;
}

std::string ItemScp860::getInvName() {
	return std::string("SCP-860");
}

std::string ItemScp860::getInvImgPath() {
	return std::string("GFX/Items/INVkey.jpg");
}

bool ItemScp860::updateItem() {
	return false;
}

void ItemScp860::updateWearing() {}
void ItemScp860::drawItem() {}
void ItemScp860::setDrawCoords(irr::core::vector2di newCoords) {}
