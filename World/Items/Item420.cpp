#include "Item420.h"

irr::scene::IMeshSceneNode* Item420::baseNode = nullptr;
btConvexHullShape* Item420::shape = nullptr;
irr::core::vector3df Item420::offset = irr::core::vector3df(0,0,0);

Item420::Item420() {}

void Item420::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (Item420::baseNode == nullptr) {
        Item420::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

Item420* Item420::createItem420() {
    if (Item420::baseNode == nullptr) return nullptr;

    if (Item420::shape == nullptr) {
		Item::createShapeFromNode(Item420::baseNode,Item420::shape,Item420::offset);
	}

    Item420* newIt = new Item420;

    newIt->loadAssets(Item420::baseNode,Item420::shape);

    return newIt;
}

irr::core::vector3df Item420::getOffset() {
	return Item420::offset;
}

std::string Item420::getInvName() {
	return std::string("Some SCP-420-J");
}

std::string Item420::getInvImgPath() {
	return std::string("GFX/Items/INV420.jpg");
}

bool Item420::updateItem() {
	return false;
}

void Item420::updateWearing() {}
void Item420::drawItem() {}
