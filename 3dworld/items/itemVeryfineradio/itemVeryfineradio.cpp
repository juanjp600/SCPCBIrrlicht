#include "itemVeryfineradio.h"

irr::scene::IMeshSceneNode* itemVeryfineradio::baseNode = nullptr;
btConvexHullShape* itemVeryfineradio::shape = nullptr;
irr::core::vector3df itemVeryfineradio::offset = irr::core::vector3df(0,0,0);

itemVeryfineradio::itemVeryfineradio() {}

void itemVeryfineradio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemVeryfineradio::baseNode == nullptr) {
        itemVeryfineradio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemVeryfineradio* itemVeryfineradio::createItemVeryfineradio() {
    if (itemVeryfineradio::baseNode == nullptr) return nullptr;

    if (itemVeryfineradio::shape == nullptr) {
		item::createShapeFromNode(itemVeryfineradio::baseNode,itemVeryfineradio::shape,itemVeryfineradio::offset);
	}

    itemVeryfineradio* newIt = new itemVeryfineradio;

    newIt->loadAssets(itemVeryfineradio::baseNode,itemVeryfineradio::shape);

    return newIt;
}

irr::core::vector3df itemVeryfineradio::getOffset() {
	return itemVeryfineradio::offset;
}

std::string itemVeryfineradio::getInvName() {
	return std::string("Radio Transceiver");
}

std::string itemVeryfineradio::getInvImgPath() {
	return std::string("GFX/items/INVradio.jpg");
}

bool itemVeryfineradio::updateItem() {
	return false;
}
