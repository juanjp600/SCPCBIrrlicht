#include "itemScp500.h"

irr::scene::IMeshSceneNode* itemScp500::baseNode = nullptr;
btConvexHullShape* itemScp500::shape = nullptr;
irr::core::vector3df itemScp500::offset = irr::core::vector3df(0,0,0);

itemScp500::itemScp500() {}

void itemScp500::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemScp500::baseNode == nullptr) {
        itemScp500::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemScp500* itemScp500::createItemScp500() {
    if (itemScp500::baseNode == nullptr) return nullptr;

    if (itemScp500::shape == nullptr) {
		item::createShapeFromNode(itemScp500::baseNode,itemScp500::shape,itemScp500::offset);
	}

    itemScp500* newIt = new itemScp500;

    newIt->loadAssets(itemScp500::baseNode,itemScp500::shape);

    return newIt;
}

irr::core::vector3df itemScp500::getOffset() {
	return itemScp500::offset;
}

std::string itemScp500::getInvName() {
	return std::string("SCP-500-01");
}

std::string itemScp500::getInvImgPath() {
	return std::string("GFX/items/INVpill.jpg");
}

bool itemScp500::updateItem() {
	return false;
}

void itemScp500::updateWearing() {}
void itemScp500::drawItem() {}
