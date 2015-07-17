#include "Item420s.h"

irr::scene::IMeshSceneNode* Item420s::baseNode = nullptr;
btConvexHullShape* Item420s::shape = nullptr;
irr::core::vector3df Item420s::offset = irr::core::vector3df(0,0,0);

Item420s::Item420s() {}

void Item420s::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (Item420s::baseNode == nullptr) {
        Item420s::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

Item420s* Item420s::createItem420s() {
    if (Item420s::baseNode == nullptr) return nullptr;

    if (Item420s::shape == nullptr) {
		Item::createShapeFromNode(Item420s::baseNode,Item420s::shape,Item420s::offset);
	}

    Item420s* newIt = new Item420s;

    newIt->loadAssets(Item420s::baseNode,Item420s::shape);

    return newIt;
}

irr::core::vector3df Item420s::getOffset() {
	return Item420s::offset;
}

std::string Item420s::getInvName() {
	return std::string("Smelly Joint");
}

std::string Item420s::getInvImgPath() {
	return std::string("GFX/Items/INV420.jpg");
}

bool Item420s::updateItem() {
	return false;
}

void Item420s::updateWearing() {}
void Item420s::drawItem() {}
void Item420s::setDrawCoords(irr::core::vector2di newCoords) {}
