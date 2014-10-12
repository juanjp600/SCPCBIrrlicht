#include "itemVeryfinefirstaid.h"

irr::scene::IMeshSceneNode* itemVeryfinefirstaid::baseNode = nullptr;
btConvexHullShape* itemVeryfinefirstaid::shape = nullptr;
irr::core::vector3df itemVeryfinefirstaid::offset = irr::core::vector3df(0,0,0);

itemVeryfinefirstaid::itemVeryfinefirstaid() {}

void itemVeryfinefirstaid::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemVeryfinefirstaid::baseNode == nullptr) {
        itemVeryfinefirstaid::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemVeryfinefirstaid* itemVeryfinefirstaid::createItemVeryfinefirstaid() {
    if (itemVeryfinefirstaid::baseNode == nullptr) return nullptr;

    if (itemVeryfinefirstaid::shape == nullptr) {
		item::createShapeFromNode(itemVeryfinefirstaid::baseNode,itemVeryfinefirstaid::shape,itemVeryfinefirstaid::offset);
	}

    itemVeryfinefirstaid* newIt = new itemVeryfinefirstaid;

    newIt->loadAssets(itemVeryfinefirstaid::baseNode,itemVeryfinefirstaid::shape);

    return newIt;
}

irr::core::vector3df itemVeryfinefirstaid::getOffset() {
	return itemVeryfinefirstaid::offset;
}

std::string itemVeryfinefirstaid::getInvName() {
	return std::string("Strange Bottle");
}

bool itemVeryfinefirstaid::updateItem() {
	return false;
}
