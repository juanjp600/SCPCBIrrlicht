#include "itemPaper.h"

irr::scene::IMeshSceneNode* itemPaper::baseNode = nullptr;
btConvexHullShape* itemPaper::shape = nullptr;
irr::core::vector3df itemPaper::offset = irr::core::vector3df(0,0,0);

itemPaper::itemPaper() {}

void itemPaper::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemPaper::baseNode == nullptr) {
        itemPaper::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemPaper* itemPaper::createItemPaper() {
    if (itemPaper::baseNode == nullptr) return nullptr;

    if (itemPaper::shape == nullptr) {
		item::createShapeFromNode(itemPaper::baseNode,itemPaper::shape,itemPaper::offset);
	}

    itemPaper* newIt = new itemPaper;

    newIt->loadAssets(itemPaper::baseNode,itemPaper::shape);

    return newIt;
}

irr::core::vector3df itemPaper::getOffset() {
	return itemPaper::offset;
}

std::string itemPaper::getInvName() {
	return std::string("Incident Report SCP-106-0204");
}

std::string itemPaper::getInvImgPath() {
	return std::string("GFX/items/INVpaper.jpg");
}

bool itemPaper::updateItem() {
	return false;
}
