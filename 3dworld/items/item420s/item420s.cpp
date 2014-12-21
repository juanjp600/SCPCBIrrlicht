#include "item420s.h"

irr::scene::IMeshSceneNode* item420s::baseNode = nullptr;
btConvexHullShape* item420s::shape = nullptr;
irr::core::vector3df item420s::offset = irr::core::vector3df(0,0,0);

item420s::item420s() {}

void item420s::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (item420s::baseNode == nullptr) {
        item420s::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

item420s* item420s::createItem420s() {
    if (item420s::baseNode == nullptr) return nullptr;

    if (item420s::shape == nullptr) {
		item::createShapeFromNode(item420s::baseNode,item420s::shape,item420s::offset);
	}

    item420s* newIt = new item420s;

    newIt->loadAssets(item420s::baseNode,item420s::shape);

    return newIt;
}

irr::core::vector3df item420s::getOffset() {
	return item420s::offset;
}

std::string item420s::getInvName() {
	return std::string("Smelly Joint");
}

std::string item420s::getInvImgPath() {
	return std::string("GFX/items/INV420.jpg");
}

bool item420s::updateItem() {
	return false;
}

void item420s::updateWearing() {}
void item420s::drawItem() {}
