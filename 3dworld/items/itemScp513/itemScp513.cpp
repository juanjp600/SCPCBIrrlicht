#include "itemScp513.h"

irr::scene::IMeshSceneNode* itemScp513::baseNode = nullptr;
btConvexHullShape* itemScp513::shape = nullptr;
irr::core::vector3df itemScp513::offset = irr::core::vector3df(0,0,0);

itemScp513::itemScp513() {}

void itemScp513::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemScp513::baseNode == nullptr) {
        itemScp513::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemScp513* itemScp513::createItemScp513() {
    if (itemScp513::baseNode == nullptr) return nullptr;

    if (itemScp513::shape == nullptr) {
		item::createShapeFromNode(itemScp513::baseNode,itemScp513::shape,itemScp513::offset);
	}

    itemScp513* newIt = new itemScp513;

    newIt->loadAssets(itemScp513::baseNode,itemScp513::shape);

    return newIt;
}

irr::core::vector3df itemScp513::getOffset() {
	return itemScp513::offset;
}

std::string itemScp513::getInvName() {
	return std::string("SCP-513");
}

bool itemScp513::updateItem() {
	return false;
}
