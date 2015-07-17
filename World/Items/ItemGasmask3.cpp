#include "ItemGasmask3.h"

irr::scene::IMeshSceneNode* ItemGasmask3::baseNode = nullptr;
btConvexHullShape* ItemGasmask3::shape = nullptr;
irr::core::vector3df ItemGasmask3::offset = irr::core::vector3df(0,0,0);

ItemGasmask3::ItemGasmask3() {}

void ItemGasmask3::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemGasmask3::baseNode == nullptr) {
        ItemGasmask3::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemGasmask3* ItemGasmask3::createItemGasmask3() {
    if (ItemGasmask3::baseNode == nullptr) return nullptr;

    if (ItemGasmask3::shape == nullptr) {
		Item::createShapeFromNode(ItemGasmask3::baseNode,ItemGasmask3::shape,ItemGasmask3::offset);
	}

    ItemGasmask3* newIt = new ItemGasmask3;

    newIt->loadAssets(ItemGasmask3::baseNode,ItemGasmask3::shape);

    return newIt;
}

irr::core::vector3df ItemGasmask3::getOffset() {
	return ItemGasmask3::offset;
}

std::string ItemGasmask3::getInvName() {
	return std::string("Heavy Gas Mask");
}

std::string ItemGasmask3::getInvImgPath() {
	return std::string("GFX/Items/INVgasmask.jpg");
}

bool ItemGasmask3::updateItem() {
	return false;
}

void ItemGasmask3::updateWearing() {}
void ItemGasmask3::drawItem() {}
void ItemGasmask3::setDrawCoords(irr::core::vector2di newCoords) {}
