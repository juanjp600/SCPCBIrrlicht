#include "itemScp148.h"

irr::scene::IMeshSceneNode* itemScp148::baseNode = nullptr;
btConvexHullShape* itemScp148::shape = nullptr;
irr::core::vector3df itemScp148::offset = irr::core::vector3df(0,0,0);

itemScp148::itemScp148() {}

void itemScp148::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemScp148::baseNode == nullptr) {
        itemScp148::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemScp148* itemScp148::createItemScp148() {
    if (itemScp148::baseNode == nullptr) return nullptr;

    if (itemScp148::shape == nullptr) {
		item::createShapeFromNode(itemScp148::baseNode,itemScp148::shape,itemScp148::offset);
	}

    itemScp148* newIt = new itemScp148;

    newIt->loadAssets(itemScp148::baseNode,itemScp148::shape);

    return newIt;
}

irr::core::vector3df itemScp148::getOffset() {
	return itemScp148::offset;
}

std::string itemScp148::getInvName() {
	return std::string("Metal Panel");
}

bool itemScp148::updateItem() {
	return false;
}
