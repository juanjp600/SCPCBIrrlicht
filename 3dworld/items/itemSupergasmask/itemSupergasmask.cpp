#include "itemSupergasmask.h"

irr::scene::IMeshSceneNode* itemSupergasmask::baseNode = nullptr;
btConvexHullShape* itemSupergasmask::shape = nullptr;
irr::core::vector3df itemSupergasmask::offset = irr::core::vector3df(0,0,0);

itemSupergasmask::itemSupergasmask() {}

void itemSupergasmask::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemSupergasmask::baseNode == nullptr) {
        itemSupergasmask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemSupergasmask* itemSupergasmask::createItemSupergasmask() {
    if (itemSupergasmask::baseNode == nullptr) return nullptr;

    if (itemSupergasmask::shape == nullptr) {
		item::createShapeFromNode(itemSupergasmask::baseNode,itemSupergasmask::shape,itemSupergasmask::offset);
	}

    itemSupergasmask* newIt = new itemSupergasmask;

    newIt->loadAssets(itemSupergasmask::baseNode,itemSupergasmask::shape);

    return newIt;
}

irr::core::vector3df itemSupergasmask::getOffset() {
	return itemSupergasmask::offset;
}

std::string itemSupergasmask::getInvName() {
	return std::string("Gas Mask");
}

bool itemSupergasmask::updateItem() {
	return false;
}
