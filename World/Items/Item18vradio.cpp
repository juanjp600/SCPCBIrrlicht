#include "Item18vradio.h"

irr::scene::IMeshSceneNode* Item18vradio::baseNode = nullptr;
btConvexHullShape* Item18vradio::shape = nullptr;
irr::core::vector3df Item18vradio::offset = irr::core::vector3df(0,0,0);

Item18vradio::Item18vradio() {}

void Item18vradio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (Item18vradio::baseNode == nullptr) {
        Item18vradio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

Item18vradio* Item18vradio::createItem18vradio() {
    if (Item18vradio::baseNode == nullptr) return nullptr;

    if (Item18vradio::shape == nullptr) {
		Item::createShapeFromNode(Item18vradio::baseNode,Item18vradio::shape,Item18vradio::offset);
	}

    Item18vradio* newIt = new Item18vradio;

    newIt->loadAssets(Item18vradio::baseNode,Item18vradio::shape);

    return newIt;
}

irr::core::vector3df Item18vradio::getOffset() {
	return Item18vradio::offset;
}

std::string Item18vradio::getInvName() {
	return std::string("Radio Transceiver");
}

std::string Item18vradio::getInvImgPath() {
	return std::string("GFX/Items/INVradio.jpg");
}

bool Item18vradio::updateItem() {
	return false;
}

void Item18vradio::updateWearing() {}
void Item18vradio::drawItem() {}
