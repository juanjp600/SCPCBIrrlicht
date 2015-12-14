#include "ItemKey2.h"

irr::scene::IMeshSceneNode* ItemKey2::baseNode = nullptr;
btConvexHullShape* ItemKey2::shape = nullptr;
irr::core::vector3df ItemKey2::offset = irr::core::vector3df(0,0,0);

ItemKey2::ItemKey2() {}

void ItemKey2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey2::baseNode == nullptr) {
        ItemKey2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey2* ItemKey2::createItemKey2() {
    if (ItemKey2::baseNode == nullptr) return nullptr;

    if (ItemKey2::shape == nullptr) {
		Item::createShapeFromNode(ItemKey2::baseNode,ItemKey2::shape,ItemKey2::offset);
	}

    ItemKey2* newIt = new ItemKey2;

    newIt->loadAssets(ItemKey2::baseNode,ItemKey2::shape);

    return newIt;
}

irr::core::vector3df ItemKey2::getOffset() {
	return ItemKey2::offset;
}

std::string ItemKey2::getInvName() {
	return std::string("Level 2 Key Card");
}

std::string ItemKey2::getInvImgPath() {
	return std::string("GFX/Items/INVkey2.jpg");
}

bool ItemKey2::updateItem() {
	return true;
}

void ItemKey2::updateWearing() {}

void ItemKey2::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(drawCoords.X-32,drawCoords.Y-32,drawCoords.X+32,drawCoords.Y+32),irr::core::recti(0,0,64,64));
}

void ItemKey2::setDrawCoords(irr::core::vector2di newCoords) {
    drawCoords = newCoords;
}

bool ItemKey2::refineItem(REFINE_SETTINGS setting,irr::core::aabbox3df intake,irr::core::aabbox3df output,Item*& result) {
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
