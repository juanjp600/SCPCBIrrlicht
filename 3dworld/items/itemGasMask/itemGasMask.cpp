#include "itemGasmask.h"

irr::scene::IMeshSceneNode* itemGasmask::baseNode = nullptr;
btConvexHullShape* itemGasmask::shape = nullptr;
irr::core::vector3df itemGasmask::offset = irr::core::vector3df(0,0,0);

itemGasmask::itemGasmask() {}

void itemGasmask::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemGasmask::baseNode == nullptr) {
        itemGasmask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemGasmask* itemGasmask::createItemGasmask() {
    if (itemGasmask::baseNode == nullptr) return nullptr;

    if (itemGasmask::shape == nullptr) {
		item::createShapeFromNode(itemGasmask::baseNode,itemGasmask::shape,itemGasmask::offset);
	}

    itemGasmask* newIt = new itemGasmask;

    newIt->loadAssets(itemGasmask::baseNode,itemGasmask::shape);

    return newIt;
}

irr::core::vector3df itemGasmask::getOffset() {
	return itemGasmask::offset;
}

std::string itemGasmask::getInvName() {
	return std::string("Gas Mask");
}

bool itemGasmask::updateItem() {
	return false;
}
