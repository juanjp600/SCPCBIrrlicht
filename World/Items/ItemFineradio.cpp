#include "ItemFineradio.h"

irr::scene::IMeshSceneNode* ItemFineradio::baseNode = nullptr;
btConvexHullShape* ItemFineradio::shape = nullptr;
irr::core::vector3df ItemFineradio::offset = irr::core::vector3df(0,0,0);

ItemFineradio::ItemFineradio() {}

void ItemFineradio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemFineradio::baseNode == nullptr) {
        ItemFineradio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemFineradio* ItemFineradio::createItemFineradio() {
    if (ItemFineradio::baseNode == nullptr) return nullptr;

    if (ItemFineradio::shape == nullptr) {
		Item::createShapeFromNode(ItemFineradio::baseNode,ItemFineradio::shape,ItemFineradio::offset);
	}

    ItemFineradio* newIt = new ItemFineradio;

    newIt->loadAssets(ItemFineradio::baseNode,ItemFineradio::shape);

    return newIt;
}

irr::core::vector3df ItemFineradio::getOffset() {
	return ItemFineradio::offset;
}

std::string ItemFineradio::getInvName() {
	return std::string("Radio Transceiver");
}

std::string ItemFineradio::getInvImgPath() {
	return std::string("GFX/Items/INVradio.jpg");
}

bool ItemFineradio::updateItem() {
	return false;
}

void ItemFineradio::updateWearing() {}
void ItemFineradio::drawItem() {}
