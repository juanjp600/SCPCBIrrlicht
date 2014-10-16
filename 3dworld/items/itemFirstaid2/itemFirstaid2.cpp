#include "itemFirstaid2.h"

irr::scene::IMeshSceneNode* itemFirstaid2::baseNode = nullptr;
btConvexHullShape* itemFirstaid2::shape = nullptr;
irr::core::vector3df itemFirstaid2::offset = irr::core::vector3df(0,0,0);

itemFirstaid2::itemFirstaid2() {}

void itemFirstaid2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemFirstaid2::baseNode == nullptr) {
        itemFirstaid2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemFirstaid2* itemFirstaid2::createItemFirstaid2() {
    if (itemFirstaid2::baseNode == nullptr) return nullptr;

    if (itemFirstaid2::shape == nullptr) {
		item::createShapeFromNode(itemFirstaid2::baseNode,itemFirstaid2::shape,itemFirstaid2::offset);
	}

    itemFirstaid2* newIt = new itemFirstaid2;

    newIt->loadAssets(itemFirstaid2::baseNode,itemFirstaid2::shape);

    return newIt;
}

irr::core::vector3df itemFirstaid2::getOffset() {
	return itemFirstaid2::offset;
}

std::string itemFirstaid2::getInvName() {
	return std::string("Blue First Aid Kit");
}

std::string itemFirstaid2::getInvImgPath() {
	return std::string("GFX/items/INVfirstaid2.jpg");
}

bool itemFirstaid2::updateItem() {
	return false;
}
