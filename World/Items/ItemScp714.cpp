#include "ItemScp714.h"

#include "../World.h"
#include "../Player.h"

irr::scene::IMeshSceneNode* ItemScp714::baseNode = nullptr;
btConvexHullShape* ItemScp714::shape = nullptr;
irr::core::vector3df ItemScp714::offset = irr::core::vector3df(0,0,0);

ItemScp714::ItemScp714() {}

void ItemScp714::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemScp714::baseNode == nullptr) {
        ItemScp714::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemScp714* ItemScp714::createItemScp714() {
    if (ItemScp714::baseNode == nullptr) return nullptr;

    if (ItemScp714::shape == nullptr) {
		Item::createShapeFromNode(ItemScp714::baseNode,ItemScp714::shape,ItemScp714::offset);
		ItemScp714::shape->setMargin(0.02f);
	}

    ItemScp714* newIt = new ItemScp714;

    newIt->loadAssets(ItemScp714::baseNode,ItemScp714::shape);

    return newIt;
}

irr::core::vector3df ItemScp714::getOffset() {
	return ItemScp714::offset;
}

std::string ItemScp714::getInvName() {
	return std::string("SCP-714");
}

std::string ItemScp714::getInvImgPath() {
	return std::string("GFX/Items/INV714.jpg");
}

bool ItemScp714::updateItem() {
    Item::mainPlayer->select714(this);
	return false;
}

void ItemScp714::updateWearing() {
    Item::mainPlayer->booststamina(0.f,-5.f);
}

void ItemScp714::drawItem() {}
void ItemScp714::setDrawCoords(irr::core::vector2di newCoords) {}
