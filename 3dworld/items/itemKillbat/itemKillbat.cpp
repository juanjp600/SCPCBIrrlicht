#include "itemKillbat.h"

irr::scene::IMeshSceneNode* itemKillbat::baseNode = nullptr;
btConvexHullShape* itemKillbat::shape = nullptr;
irr::core::vector3df itemKillbat::offset = irr::core::vector3df(0,0,0);

itemKillbat::itemKillbat() {}

void itemKillbat::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemKillbat::baseNode == nullptr) {
        itemKillbat::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemKillbat* itemKillbat::createItemKillbat() {
    if (itemKillbat::baseNode == nullptr) return nullptr;

    if (itemKillbat::shape == nullptr) {
		item::createShapeFromNode(itemKillbat::baseNode,itemKillbat::shape,itemKillbat::offset);
	}

    itemKillbat* newIt = new itemKillbat;

    newIt->loadAssets(itemKillbat::baseNode,itemKillbat::shape);

    return newIt;
}

irr::core::vector3df itemKillbat::getOffset() {
	return itemKillbat::offset;
}

std::string itemKillbat::getInvName() {
	return std::string("Strange Battery");
}

bool itemKillbat::updateItem() {
	return false;
}
