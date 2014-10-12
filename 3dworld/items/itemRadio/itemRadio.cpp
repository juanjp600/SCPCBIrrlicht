#include "itemRadio.h"

irr::scene::IMeshSceneNode* itemRadio::baseNode = nullptr;
btConvexHullShape* itemRadio::shape = nullptr;
irr::core::vector3df itemRadio::offset = irr::core::vector3df(0,0,0);

itemRadio::itemRadio() {}

void itemRadio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemRadio::baseNode == nullptr) {
        itemRadio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemRadio* itemRadio::createItemRadio() {
    if (itemRadio::baseNode == nullptr) return nullptr;

    if (itemRadio::shape == nullptr) {
		item::createShapeFromNode(itemRadio::baseNode,itemRadio::shape,itemRadio::offset);
	}

    itemRadio* newIt = new itemRadio;

    newIt->loadAssets(itemRadio::baseNode,itemRadio::shape);

    return newIt;
}

irr::core::vector3df itemRadio::getOffset() {
	return itemRadio::offset;
}

std::string itemRadio::getInvName() {
	return std::string("Radio Transceiver");
}

bool itemRadio::updateItem() {
	return false;
}
