#include "itemFineradio.h"

irr::scene::IMeshSceneNode* itemFineradio::baseNode = nullptr;
btConvexHullShape* itemFineradio::shape = nullptr;
irr::core::vector3df itemFineradio::offset = irr::core::vector3df(0,0,0);

itemFineradio::itemFineradio() {}

void itemFineradio::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemFineradio::baseNode == nullptr) {
        itemFineradio::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemFineradio* itemFineradio::createItemFineradio() {
    if (itemFineradio::baseNode == nullptr) return nullptr;

    if (itemFineradio::shape == nullptr) {
		item::createShapeFromNode(itemFineradio::baseNode,itemFineradio::shape,itemFineradio::offset);
	}

    itemFineradio* newIt = new itemFineradio;

    newIt->loadAssets(itemFineradio::baseNode,itemFineradio::shape);

    return newIt;
}

irr::core::vector3df itemFineradio::getOffset() {
	return itemFineradio::offset;
}

std::string itemFineradio::getInvName() {
	return std::string("Radio Transceiver");
}

std::string itemFineradio::getInvImgPath() {
	return std::string("GFX/items/INVradio.jpg");
}

bool itemFineradio::updateItem() {
	return false;
}

void itemFineradio::updateWearing() {}
void itemFineradio::drawItem() {}
