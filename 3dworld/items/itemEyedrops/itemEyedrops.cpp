#include "itemEyedrops.h"

irr::scene::IMeshSceneNode* itemEyedrops::baseNode = nullptr;
btConvexHullShape* itemEyedrops::shape = nullptr;
irr::core::vector3df itemEyedrops::offset = irr::core::vector3df(0,0,0);

itemEyedrops::itemEyedrops() {}

void itemEyedrops::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemEyedrops::baseNode == nullptr) {
        itemEyedrops::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemEyedrops* itemEyedrops::createItemEyedrops() {
    if (itemEyedrops::baseNode == nullptr) return nullptr;

    if (itemEyedrops::shape == nullptr) {
		item::createShapeFromNode(itemEyedrops::baseNode,itemEyedrops::shape,itemEyedrops::offset);
	}

    itemEyedrops* newIt = new itemEyedrops;

    newIt->loadAssets(itemEyedrops::baseNode,itemEyedrops::shape);

    return newIt;
}

irr::core::vector3df itemEyedrops::getOffset() {
	return itemEyedrops::offset;
}

std::string itemEyedrops::getInvName() {
	return std::string("RedVision Eyedrops");
}

bool itemEyedrops::updateItem() {
	return false;
}
