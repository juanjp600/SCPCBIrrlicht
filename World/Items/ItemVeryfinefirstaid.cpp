#include "ItemVeryfinefirstaid.h"

irr::scene::IMeshSceneNode* ItemVeryfinefirstaid::baseNode = nullptr;
btConvexHullShape* ItemVeryfinefirstaid::shape = nullptr;
irr::core::vector3df ItemVeryfinefirstaid::offset = irr::core::vector3df(0,0,0);

ItemVeryfinefirstaid::ItemVeryfinefirstaid() {}

void ItemVeryfinefirstaid::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemVeryfinefirstaid::baseNode == nullptr) {
        ItemVeryfinefirstaid::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemVeryfinefirstaid* ItemVeryfinefirstaid::createItemVeryfinefirstaid() {
    if (ItemVeryfinefirstaid::baseNode == nullptr) return nullptr;

    if (ItemVeryfinefirstaid::shape == nullptr) {
		Item::createShapeFromNode(ItemVeryfinefirstaid::baseNode,ItemVeryfinefirstaid::shape,ItemVeryfinefirstaid::offset);
	}

    ItemVeryfinefirstaid* newIt = new ItemVeryfinefirstaid;

    newIt->loadAssets(ItemVeryfinefirstaid::baseNode,ItemVeryfinefirstaid::shape);

    return newIt;
}

irr::core::vector3df ItemVeryfinefirstaid::getOffset() {
	return ItemVeryfinefirstaid::offset;
}

std::string ItemVeryfinefirstaid::getInvName() {
	return std::string("Strange Bottle");
}

std::string ItemVeryfinefirstaid::getInvImgPath() {
	return std::string("GFX/Items/INVbottle.jpg");
}

bool ItemVeryfinefirstaid::updateItem() {
	return false;
}

void ItemVeryfinefirstaid::updateWearing() {}
void ItemVeryfinefirstaid::drawItem() {}
