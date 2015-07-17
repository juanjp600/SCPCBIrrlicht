#include "ItemVeryfineradio.h"

irr::scene::IMeshSceneNode* ItemVeryfineradio::baseNode = nullptr;
btConvexHullShape* ItemVeryfineradio::shape = nullptr;
irr::core::vector3df ItemVeryfineradio::offset = irr::core::vector3df(0,0,0);

ItemVeryfineradio::ItemVeryfineradio() {}

void ItemVeryfineradio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemVeryfineradio::baseNode == nullptr) {
        ItemVeryfineradio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemVeryfineradio* ItemVeryfineradio::createItemVeryfineradio() {
    if (ItemVeryfineradio::baseNode == nullptr) return nullptr;

    if (ItemVeryfineradio::shape == nullptr) {
		Item::createShapeFromNode(ItemVeryfineradio::baseNode,ItemVeryfineradio::shape,ItemVeryfineradio::offset);
	}

    ItemVeryfineradio* newIt = new ItemVeryfineradio;

    newIt->loadAssets(ItemVeryfineradio::baseNode,ItemVeryfineradio::shape);

    return newIt;
}

irr::core::vector3df ItemVeryfineradio::getOffset() {
	return ItemVeryfineradio::offset;
}

std::string ItemVeryfineradio::getInvName() {
	return std::string("Radio Transceiver");
}

std::string ItemVeryfineradio::getInvImgPath() {
	return std::string("GFX/Items/INVradio.jpg");
}

bool ItemVeryfineradio::updateItem() {
	return false;
}

void ItemVeryfineradio::updateWearing() {}
void ItemVeryfineradio::drawItem() {}
void ItemVeryfineradio::setDrawCoords(irr::core::vector2di newCoords) {}
