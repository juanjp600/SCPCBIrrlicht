#include "itemMisc.h"

irr::scene::IMeshSceneNode* itemMisc::baseNode = nullptr;
btConvexHullShape* itemMisc::shape = nullptr;
irr::core::vector3df itemMisc::offset = irr::core::vector3df(0,0,0);

itemMisc::itemMisc() {}

void itemMisc::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemMisc::baseNode == nullptr) {
        itemMisc::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemMisc* itemMisc::createItemMisc() {
    if (itemMisc::baseNode == nullptr) return nullptr;

    if (itemMisc::shape == nullptr) {
		item::createShapeFromNode(itemMisc::baseNode,itemMisc::shape,itemMisc::offset);
	}

    itemMisc* newIt = new itemMisc;

    newIt->loadAssets(itemMisc::baseNode,itemMisc::shape);

    return newIt;
}

irr::core::vector3df itemMisc::getOffset() {
	return itemMisc::offset;
}

std::string itemMisc::getInvName() {
	return std::string("Electronical components");
}

std::string itemMisc::getInvImgPath() {
	return std::string("GFX/items/INVelectronics.jpg");
}

bool itemMisc::updateItem() {
	return false;
}

void itemMisc::updateWearing() {}
void itemMisc::drawItem() {}
