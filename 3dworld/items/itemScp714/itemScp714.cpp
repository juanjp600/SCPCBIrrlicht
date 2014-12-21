#include "itemScp714.h"

#include "../../3dworld.h"
#include "../../player.h"

irr::scene::IMeshSceneNode* itemScp714::baseNode = nullptr;
btConvexHullShape* itemScp714::shape = nullptr;
irr::core::vector3df itemScp714::offset = irr::core::vector3df(0,0,0);

itemScp714::itemScp714() {}

void itemScp714::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemScp714::baseNode == nullptr) {
        itemScp714::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemScp714* itemScp714::createItemScp714() {
    if (itemScp714::baseNode == nullptr) return nullptr;

    if (itemScp714::shape == nullptr) {
		item::createShapeFromNode(itemScp714::baseNode,itemScp714::shape,itemScp714::offset);
	}

    itemScp714* newIt = new itemScp714;

    newIt->loadAssets(itemScp714::baseNode,itemScp714::shape);

    return newIt;
}

irr::core::vector3df itemScp714::getOffset() {
	return itemScp714::offset;
}

std::string itemScp714::getInvName() {
	return std::string("SCP-714");
}

std::string itemScp714::getInvImgPath() {
	return std::string("GFX/items/INV714.jpg");
}

bool itemScp714::updateItem() {
    item::mainPlayer->select714(this);
	return false;
}

void itemScp714::updateWearing() {
    item::mainPlayer->boostStamina(0.f,-20.f);
}

void itemScp714::drawItem() {}
