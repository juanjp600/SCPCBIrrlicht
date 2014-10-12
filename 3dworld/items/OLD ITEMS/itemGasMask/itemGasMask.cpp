#include "itemGasMask.h"

irr::scene::IMeshSceneNode* itemGasMask::baseNode = nullptr;
btConvexHullShape* itemGasMask::shape = nullptr;
irr::core::vector3df itemGasMask::offset = irr::core::vector3df(0,0,0);
//dynRegister* itemGasMask::dynamics = nullptr;

itemGasMask::itemGasMask() {}

void itemGasMask::setMeshNode(irr::scene::IMeshSceneNode* &node) {
    if (itemGasMask::baseNode == nullptr) {
        itemGasMask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemGasMask* itemGasMask::createItemGasMask() {
    if (itemGasMask::baseNode == nullptr) return nullptr;

    if (itemGasMask::shape == nullptr) {
		item::createShapeFromNode(itemGasMask::baseNode,itemGasMask::shape,itemGasMask::offset);
    }

    itemGasMask* newIt = new itemGasMask;

    newIt->loadAssets(itemGasMask::baseNode,itemGasMask::shape);

    return newIt;
}

irr::core::vector3df itemGasMask::getOffset() {
	return itemGasMask::offset;
}
