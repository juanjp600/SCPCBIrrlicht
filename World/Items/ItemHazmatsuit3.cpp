#include "ItemHazmatsuit3.h"

irr::scene::IMeshSceneNode* ItemHazmatsuit3::baseNode = nullptr;
btConvexHullShape* ItemHazmatsuit3::shape = nullptr;
irr::core::vector3df ItemHazmatsuit3::offset = irr::core::vector3df(0,0,0);

ItemHazmatsuit3::ItemHazmatsuit3() {}

void ItemHazmatsuit3::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemHazmatsuit3::baseNode == nullptr) {
        ItemHazmatsuit3::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemHazmatsuit3* ItemHazmatsuit3::createItemHazmatsuit3() {
    if (ItemHazmatsuit3::baseNode == nullptr) return nullptr;

    if (ItemHazmatsuit3::shape == nullptr) {
		Item::createShapeFromNode(ItemHazmatsuit3::baseNode,ItemHazmatsuit3::shape,ItemHazmatsuit3::offset);
	}

    ItemHazmatsuit3* newIt = new ItemHazmatsuit3;

    newIt->loadAssets(ItemHazmatsuit3::baseNode,ItemHazmatsuit3::shape);

    return newIt;
}

irr::core::vector3df ItemHazmatsuit3::getOffset() {
	return ItemHazmatsuit3::offset;
}

std::string ItemHazmatsuit3::getInvName() {
	return std::string("Heavy Hazmat Suit");
}

std::string ItemHazmatsuit3::getInvImgPath() {
	return std::string("GFX/Items/INVhazmat.jpg");
}

bool ItemHazmatsuit3::updateItem() {
	return false;
}

void ItemHazmatsuit3::updateWearing() {}
void ItemHazmatsuit3::drawItem() {}
void ItemHazmatsuit3::setDrawCoords(irr::core::vector2di newCoords) {}
