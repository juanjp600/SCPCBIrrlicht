#include "itemScp1025.h"

irr::scene::IMeshSceneNode* itemScp1025::baseNode = nullptr;
btConvexHullShape* itemScp1025::shape = nullptr;
irr::core::vector3df itemScp1025::offset = irr::core::vector3df(0,0,0);

itemScp1025::itemScp1025() {}

void itemScp1025::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemScp1025::baseNode == nullptr) {
        itemScp1025::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemScp1025* itemScp1025::createItemScp1025() {
    if (itemScp1025::baseNode == nullptr) return nullptr;

    if (itemScp1025::shape == nullptr) {
		item::createShapeFromNode(itemScp1025::baseNode,itemScp1025::shape,itemScp1025::offset);
	}

    itemScp1025* newIt = new itemScp1025;

    newIt->loadAssets(itemScp1025::baseNode,itemScp1025::shape);

    return newIt;
}

irr::core::vector3df itemScp1025::getOffset() {
	return itemScp1025::offset;
}

std::string itemScp1025::getInvName() {
	return std::string("SCP-1025");
}

std::string itemScp1025::getInvImgPath() {
	return std::string("GFX/items/INV1025.jpg");
}

bool itemScp1025::updateItem() {
	return false;
}

void itemScp1025::updateWearing() {}
void itemScp1025::drawItem() {}
