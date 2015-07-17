#include "ItemRadio.h"

irr::scene::IMeshSceneNode* ItemRadio::baseNode = nullptr;
btConvexHullShape* ItemRadio::shape = nullptr;
irr::core::vector3df ItemRadio::offset = irr::core::vector3df(0,0,0);

ItemRadio::ItemRadio() {}

void ItemRadio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemRadio::baseNode == nullptr) {
        ItemRadio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemRadio* ItemRadio::createItemRadio() {
    if (ItemRadio::baseNode == nullptr) return nullptr;

    if (ItemRadio::shape == nullptr) {
		Item::createShapeFromNode(ItemRadio::baseNode,ItemRadio::shape,ItemRadio::offset);
	}

    ItemRadio* newIt = new ItemRadio;

    newIt->loadAssets(ItemRadio::baseNode,ItemRadio::shape);

    return newIt;
}

irr::core::vector3df ItemRadio::getOffset() {
	return ItemRadio::offset;
}

std::string ItemRadio::getInvName() {
	return std::string("Radio Transceiver");
}

std::string ItemRadio::getInvImgPath() {
	return std::string("GFX/Items/INVradio.jpg");
}

bool ItemRadio::updateItem() {
	return false;
}

void ItemRadio::updateWearing() {}
void ItemRadio::drawItem() {}
void ItemRadio::setDrawCoords(irr::core::vector2di newCoords) {}
