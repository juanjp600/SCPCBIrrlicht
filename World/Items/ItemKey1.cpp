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

bool ItemKey1::refineItem(REFINE_SETTINGS setting,irr::core::aabbox3df intake,irr::core::aabbox3df output,Item*& result) {
    result = this;
    if (intake.isPointInside(getPosition()) && !picked) {
        irr::core::vector3df newPos;
        newPos.X = ((float)(rand()%10000))*0.0001f;
        newPos.X = (newPos.X*output.MinEdge.X)+((1.f-newPos.X)*output.MaxEdge.X);
        newPos.Y = ((float)(rand()%10000))*0.0001f;
        newPos.Y = (newPos.Y*output.MinEdge.Y)+((1.f-newPos.Y)*output.MaxEdge.Y);
        newPos.Z = ((float)(rand()%10000))*0.0001f;
        newPos.Z = (newPos.Z*output.MinEdge.Z)+((1.f-newPos.Z)*output.MaxEdge.Z);
        switch (setting) {
            case REFINE_SETTINGS::FINE:
                result = ItemKey2::createItemKey2();
                result->pick();
                result->unpick(newPos);
                destroy();
                return true;
            break;
            case REFINE_SETTINGS::VERYFINE:
                result = ItemKey3::createItemKey3();
                result->pick();
                result->unpick(newPos);
                destroy();
                return true;
            break;
            default:
                pick();
                unpick(newPos);
            break;
        }

    }
    return false;
}
