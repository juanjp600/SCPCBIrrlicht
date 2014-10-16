#include "itemScp860.h"

irr::scene::IMeshSceneNode* itemScp860::baseNode = nullptr;
btConvexHullShape* itemScp860::shape = nullptr;
irr::core::vector3df itemScp860::offset = irr::core::vector3df(0,0,0);

itemScp860::itemScp860() {}

void itemScp860::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemScp860::baseNode == nullptr) {
        itemScp860::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemScp860* itemScp860::createItemScp860() {
    if (itemScp860::baseNode == nullptr) return nullptr;

    if (itemScp860::shape == nullptr) {
		item::createShapeFromNode(itemScp860::baseNode,itemScp860::shape,itemScp860::offset);
	}

    itemScp860* newIt = new itemScp860;

    newIt->loadAssets(itemScp860::baseNode,itemScp860::shape);

    return newIt;
}

irr::core::vector3df itemScp860::getOffset() {
	return itemScp860::offset;
}

std::string itemScp860::getInvName() {
	return std::string("SCP-860");
}

std::string itemScp860::getInvImgPath() {
	return std::string("GFX/items/INVkey.jpg");
}

bool itemScp860::updateItem() {
	return false;
}
