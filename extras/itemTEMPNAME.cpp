#include "itemTEMPNAME.h"

irr::scene::IMeshSceneNode* itemTEMPNAME::baseNode = nullptr;
btConvexHullShape* itemTEMPNAME::shape = nullptr;
irr::core::vector3df itemTEMPNAME::offset = irr::core::vector3df(0,0,0);

itemTEMPNAME::itemTEMPNAME() {}

void itemTEMPNAME::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemTEMPNAME::baseNode == nullptr) {
        itemTEMPNAME::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemTEMPNAME* itemTEMPNAME::createItemTEMPNAME() {
    if (itemTEMPNAME::baseNode == nullptr) return nullptr;

    if (itemTEMPNAME::shape == nullptr) {
		item::createShapeFromNode(itemTEMPNAME::baseNode,itemTEMPNAME::shape,itemTEMPNAME::offset);
	}

    itemTEMPNAME* newIt = new itemTEMPNAME;

    newIt->loadAssets(itemTEMPNAME::baseNode,itemTEMPNAME::shape);

    return newIt;
}

irr::core::vector3df itemTEMPNAME::getOffset() {
	return itemTEMPNAME::offset;
}

std::string itemTEMPNAME::getInvName() {
	return std::string("ITEMNAME");
}

bool itemTEMPNAME::updateItem() {
	return false;
}