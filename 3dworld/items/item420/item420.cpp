#include "item420.h"

irr::scene::IMeshSceneNode* item420::baseNode = nullptr;
btConvexHullShape* item420::shape = nullptr;
irr::core::vector3df item420::offset = irr::core::vector3df(0,0,0);

item420::item420() {}

void item420::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (item420::baseNode == nullptr) {
        item420::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

item420* item420::createItem420() {
    if (item420::baseNode == nullptr) return nullptr;

    if (item420::shape == nullptr) {
		item::createShapeFromNode(item420::baseNode,item420::shape,item420::offset);
	}

    item420* newIt = new item420;

    newIt->loadAssets(item420::baseNode,item420::shape);

    return newIt;
}

irr::core::vector3df item420::getOffset() {
	return item420::offset;
}

std::string item420::getInvName() {
	return std::string("Some SCP-420-J");
}

std::string item420::getInvImgPath() {
	return std::string("GFX/items/INV420.jpg");
}

bool item420::updateItem() {
	return false;
}
