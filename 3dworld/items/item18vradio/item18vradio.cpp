#include "item18vradio.h"

irr::scene::IMeshSceneNode* item18vradio::baseNode = nullptr;
btConvexHullShape* item18vradio::shape = nullptr;
irr::core::vector3df item18vradio::offset = irr::core::vector3df(0,0,0);

item18vradio::item18vradio() {}

void item18vradio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (item18vradio::baseNode == nullptr) {
        item18vradio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

item18vradio* item18vradio::createItem18vradio() {
    if (item18vradio::baseNode == nullptr) return nullptr;

    if (item18vradio::shape == nullptr) {
		item::createShapeFromNode(item18vradio::baseNode,item18vradio::shape,item18vradio::offset);
	}

    item18vradio* newIt = new item18vradio;

    newIt->loadAssets(item18vradio::baseNode,item18vradio::shape);

    return newIt;
}

irr::core::vector3df item18vradio::getOffset() {
	return item18vradio::offset;
}

std::string item18vradio::getInvName() {
	return std::string("Radio Transceiver");
}

std::string item18vradio::getInvImgPath() {
	return std::string("GFX/items/INVradio.jpg");
}

bool item18vradio::updateItem() {
	return false;
}
