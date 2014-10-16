#include "itemCigarette.h"

irr::scene::IMeshSceneNode* itemCigarette::baseNode = nullptr;
btConvexHullShape* itemCigarette::shape = nullptr;
irr::core::vector3df itemCigarette::offset = irr::core::vector3df(0,0,0);

itemCigarette::itemCigarette() {}

void itemCigarette::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemCigarette::baseNode == nullptr) {
        itemCigarette::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemCigarette* itemCigarette::createItemCigarette() {
    if (itemCigarette::baseNode == nullptr) return nullptr;

    if (itemCigarette::shape == nullptr) {
		item::createShapeFromNode(itemCigarette::baseNode,itemCigarette::shape,itemCigarette::offset);
	}

    itemCigarette* newIt = new itemCigarette;

    newIt->loadAssets(itemCigarette::baseNode,itemCigarette::shape);

    return newIt;
}

irr::core::vector3df itemCigarette::getOffset() {
	return itemCigarette::offset;
}

std::string itemCigarette::getInvName() {
	return std::string("Cigarette");
}

std::string itemCigarette::getInvImgPath() {
	return std::string("GFX/items/INV420.jpg");
}

bool itemCigarette::updateItem() {
	return false;
}
