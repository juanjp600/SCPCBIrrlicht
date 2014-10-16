#include "itemFinefirstaid.h"

irr::scene::IMeshSceneNode* itemFinefirstaid::baseNode = nullptr;
btConvexHullShape* itemFinefirstaid::shape = nullptr;
irr::core::vector3df itemFinefirstaid::offset = irr::core::vector3df(0,0,0);

itemFinefirstaid::itemFinefirstaid() {}

void itemFinefirstaid::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemFinefirstaid::baseNode == nullptr) {
        itemFinefirstaid::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemFinefirstaid* itemFinefirstaid::createItemFinefirstaid() {
    if (itemFinefirstaid::baseNode == nullptr) return nullptr;

    if (itemFinefirstaid::shape == nullptr) {
		item::createShapeFromNode(itemFinefirstaid::baseNode,itemFinefirstaid::shape,itemFinefirstaid::offset);
	}

    itemFinefirstaid* newIt = new itemFinefirstaid;

    newIt->loadAssets(itemFinefirstaid::baseNode,itemFinefirstaid::shape);

    return newIt;
}

irr::core::vector3df itemFinefirstaid::getOffset() {
	return itemFinefirstaid::offset;
}

std::string itemFinefirstaid::getInvName() {
	return std::string("Small First Aid Kit");
}

std::string itemFinefirstaid::getInvImgPath() {
	return std::string("GFX/items/INVfirstaid.jpg");
}

bool itemFinefirstaid::updateItem() {
	return false;
}
