#include "ItemKey5.h"

irr::scene::IMeshSceneNode* ItemKey5::baseNode = nullptr;
btConvexHullShape* ItemKey5::shape = nullptr;
irr::core::vector3df ItemKey5::offset = irr::core::vector3df(0,0,0);

ItemKey5::ItemKey5() {}

void ItemKey5::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey5::baseNode == nullptr) {
        ItemKey5::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey5* ItemKey5::createItemKey5() {
    if (ItemKey5::baseNode == nullptr) return nullptr;

    if (ItemKey5::shape == nullptr) {
		Item::createShapeFromNode(ItemKey5::baseNode,ItemKey5::shape,ItemKey5::offset);
	}

    ItemKey5* newIt = new ItemKey5;

    newIt->loadAssets(ItemKey5::baseNode,ItemKey5::shape);

    return newIt;
}

irr::core::vector3df ItemKey5::getOffset() {
	return ItemKey5::offset;
}

std::string ItemKey5::getInvName() {
	return std::string("Level 5 Key Card");
}

std::string ItemKey5::getInvImgPath() {
	return std::string("GFX/Items/INVkey5.jpg");
}

bool ItemKey5::updateItem() {
	return true;
}

void ItemKey5::updateWearing() {}

void ItemKey5::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(drawCoords.X-32,drawCoords.Y-32,drawCoords.X+32,drawCoords.Y+32),irr::core::recti(0,0,64,64));
}

void ItemKey5::setDrawCoords(irr::core::vector2di newCoords) {
    drawCoords = newCoords;
}

bool ItemKey5::refineItem(REFINE_SETTINGS setting,irr::core::aabbox3df intake,irr::core::aabbox3df output,Item*& result) {
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
                result = ItemKey6::createItemKey6();
                result->pick();
                result->unpick(newPos);
                destroy();
                return true;
            break;
            case REFINE_SETTINGS::VERYFINE:
                if (rand()%4>=3) {
                    result = ItemKey6::createItemKey6();
                } else {
                    result = ItemMisc::createItemMisc();
                }
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
