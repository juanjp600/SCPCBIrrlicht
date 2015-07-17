#include "ItemKillbat.h"

irr::scene::IMeshSceneNode* ItemKillbat::baseNode = nullptr;
btConvexHullShape* ItemKillbat::shape = nullptr;
irr::core::vector3df ItemKillbat::offset = irr::core::vector3df(0,0,0);

ItemKillbat::ItemKillbat() {}

void ItemKillbat::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKillbat::baseNode == nullptr) {
        ItemKillbat::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKillbat* ItemKillbat::createItemKillbat() {
    if (ItemKillbat::baseNode == nullptr) return nullptr;

    if (ItemKillbat::shape == nullptr) {
		Item::createShapeFromNode(ItemKillbat::baseNode,ItemKillbat::shape,ItemKillbat::offset);
	}

    ItemKillbat* newIt = new ItemKillbat;

    newIt->loadAssets(ItemKillbat::baseNode,ItemKillbat::shape);

    return newIt;
}

irr::core::vector3df ItemKillbat::getOffset() {
	return ItemKillbat::offset;
}

std::string ItemKillbat::getInvName() {
	return std::string("Strange Battery");
}

std::string ItemKillbat::getInvImgPath() {
	return std::string("GFX/Items/INVbattery.jpg");
}

bool ItemKillbat::updateItem() {
	return false;
}

void ItemKillbat::updateWearing() {}
void ItemKillbat::drawItem() {}
void ItemKillbat::setDrawCoords(irr::core::vector2di newCoords) {}
