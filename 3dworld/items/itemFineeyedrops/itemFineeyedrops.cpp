#include "itemFineeyedrops.h"

irr::scene::IMeshSceneNode* itemFineeyedrops::baseNode = nullptr;
btConvexHullShape* itemFineeyedrops::shape = nullptr;
irr::core::vector3df itemFineeyedrops::offset = irr::core::vector3df(0,0,0);

itemFineeyedrops::itemFineeyedrops() {}

void itemFineeyedrops::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemFineeyedrops::baseNode == nullptr) {
        itemFineeyedrops::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemFineeyedrops* itemFineeyedrops::createItemFineeyedrops() {
    if (itemFineeyedrops::baseNode == nullptr) return nullptr;

    if (itemFineeyedrops::shape == nullptr) {
		item::createShapeFromNode(itemFineeyedrops::baseNode,itemFineeyedrops::shape,itemFineeyedrops::offset);
	}

    itemFineeyedrops* newIt = new itemFineeyedrops;

    newIt->loadAssets(itemFineeyedrops::baseNode,itemFineeyedrops::shape);

    return newIt;
}

irr::core::vector3df itemFineeyedrops::getOffset() {
	return itemFineeyedrops::offset;
}

std::string itemFineeyedrops::getInvName() {
	return std::string("Eyedrops");
}

std::string itemFineeyedrops::getInvImgPath() {
	return std::string("GFX/items/INVeyedrops.jpg");
}

bool itemFineeyedrops::updateItem() {
	return false;
}
