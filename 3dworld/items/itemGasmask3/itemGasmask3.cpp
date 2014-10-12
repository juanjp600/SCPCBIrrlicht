#include "itemGasmask3.h"

irr::scene::IMeshSceneNode* itemGasmask3::baseNode = nullptr;
btConvexHullShape* itemGasmask3::shape = nullptr;
irr::core::vector3df itemGasmask3::offset = irr::core::vector3df(0,0,0);

itemGasmask3::itemGasmask3() {}

void itemGasmask3::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemGasmask3::baseNode == nullptr) {
        itemGasmask3::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemGasmask3* itemGasmask3::createItemGasmask3() {
    if (itemGasmask3::baseNode == nullptr) return nullptr;

    if (itemGasmask3::shape == nullptr) {
		item::createShapeFromNode(itemGasmask3::baseNode,itemGasmask3::shape,itemGasmask3::offset);
	}

    itemGasmask3* newIt = new itemGasmask3;

    newIt->loadAssets(itemGasmask3::baseNode,itemGasmask3::shape);

    return newIt;
}

irr::core::vector3df itemGasmask3::getOffset() {
	return itemGasmask3::offset;
}

std::string itemGasmask3::getInvName() {
	return std::string("Heavy Gas Mask");
}

bool itemGasmask3::updateItem() {
	return false;
}
