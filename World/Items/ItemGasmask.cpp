#include "ItemGasmask.h"

#include "../World.h"
#include "../Player.h"

irr::scene::IMeshSceneNode* ItemGasmask::baseNode = nullptr;
btConvexHullShape* ItemGasmask::shape = nullptr;
irr::core::vector3df ItemGasmask::offset = irr::core::vector3df(0,0,0);

ItemGasmask::ItemGasmask() {}

void ItemGasmask::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemGasmask::baseNode == nullptr) {
        ItemGasmask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemGasmask* ItemGasmask::createItemGasmask() {
    if (ItemGasmask::baseNode == nullptr) return nullptr;

    if (ItemGasmask::shape == nullptr) {
		Item::createShapeFromNode(ItemGasmask::baseNode,ItemGasmask::shape,ItemGasmask::offset);
	}

    ItemGasmask* newIt = new ItemGasmask;

    newIt->loadAssets(ItemGasmask::baseNode,ItemGasmask::shape);

    return newIt;
}

irr::core::vector3df ItemGasmask::getOffset() {
	return ItemGasmask::offset;
}

std::string ItemGasmask::getInvName() {
	return std::string("Gas Mask");
}

std::string ItemGasmask::getInvImgPath() {
	return std::string("GFX/Items/INVgasmask.jpg");
}

bool ItemGasmask::updateItem() {
    Item::mainPlayer->selectGasMask(this);
	return false;
}

void ItemGasmask::updateWearing() {}

void ItemGasmask::drawItem() {
    irr::video::ITexture* maskTex = Item::irrDriver->getTexture("GFX/GasmaskOverlay.png");
    unsigned short width = maskTex->getSize().Width;
    unsigned short height = maskTex->getSize().Height;
    Item::irrDriver->draw2DImage(maskTex,irr::core::recti(0,(Item::screenHeight/2)-((height/2)*screenWidth/screenHeight*0.7f),Item::screenWidth,(Item::screenHeight/2)+((height/2)*screenWidth/screenHeight*0.7f)),irr::core::recti(0,0,width,height),nullptr,nullptr,true);
}

void ItemGasmask::setDrawCoords(irr::core::vector2di newCoords) {}

bool ItemGasmask::refineItem(REFINE_SETTINGS setting,irr::core::aabbox3df intake,irr::core::aabbox3df output,Item*& result) {
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
                result = ItemSupergasmask::createItemSupergasmask();
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
