#include "itemFirstaid.h"

irr::scene::IMeshSceneNode* itemFirstaid::baseNode = nullptr;
btConvexHullShape* itemFirstaid::shape = nullptr;
irr::core::vector3df itemFirstaid::offset = irr::core::vector3df(0,0,0);

itemFirstaid::itemFirstaid() {}

void itemFirstaid::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemFirstaid::baseNode == nullptr) {
        itemFirstaid::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemFirstaid* itemFirstaid::createItemFirstaid() {
    if (itemFirstaid::baseNode == nullptr) return nullptr;

    if (itemFirstaid::shape == nullptr) {
		item::createShapeFromNode(itemFirstaid::baseNode,itemFirstaid::shape,itemFirstaid::offset);
	}

    itemFirstaid* newIt = new itemFirstaid;

    newIt->loadAssets(itemFirstaid::baseNode,itemFirstaid::shape);

    return newIt;
}

irr::core::vector3df itemFirstaid::getOffset() {
	return itemFirstaid::offset;
}

std::string itemFirstaid::getInvName() {
	return std::string("First Aid Kit");
}

bool itemFirstaid::updateItem() {
	return false;
}
