#include "ItemKey1.h"

irr::scene::IMeshSceneNode* ItemKey1::baseNode = nullptr;
btConvexHullShape* ItemKey1::shape = nullptr;
irr::core::vector3df ItemKey1::offset = irr::core::vector3df(0,0,0);

ItemKey1::ItemKey1() {}

void ItemKey1::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey1::baseNode == nullptr) {
        ItemKey1::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey1* ItemKey1::createItemKey1() {
    if (ItemKey1::baseNode == nullptr) return nullptr;

    if (ItemKey1::shape == nullptr) {
		Item::createShapeFromNode(ItemKey1::baseNode,ItemKey1::shape,ItemKey1::offset);
	}

    ItemKey1* newIt = new ItemKey1;

    newIt->loadAssets(ItemKey1::baseNode,ItemKey1::shape);

    return newIt;
}

irr::core::vector3df ItemKey1::getOffset() {
	return ItemKey1::offset;
}

std::string ItemKey1::getInvName() {
	return std::string("Level 1 Key Card");
}

std::string ItemKey1::getInvImgPath() {
	return std::string("GFX/Items/INVkey1.jpg");
}

bool ItemKey1::updateItem() {
	return true;
}

void ItemKey1::updateWearing() {}

void ItemKey1::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(drawCoords.X-32,drawCoords.Y-32,drawCoords.X+32,drawCoords.Y+32),irr::core::recti(0,0,64,64));
}

void ItemKey1::setDrawCoords(irr::core::vector2di newCoords) {
    drawCoords = newCoords;
}
