#include "ItemPaper.h"

irr::scene::IMeshSceneNode* ItemPaper::baseNode = nullptr;
btConvexHullShape* ItemPaper::shape = nullptr;
irr::core::vector3df ItemPaper::offset = irr::core::vector3df(0,0,0);

ItemPaper::ItemPaper() {}

void ItemPaper::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemPaper::baseNode == nullptr) {
        ItemPaper::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemPaper* ItemPaper::createItemPaper() {
    if (ItemPaper::baseNode == nullptr) return nullptr;

    if (ItemPaper::shape == nullptr) {
		Item::createShapeFromNode(ItemPaper::baseNode,ItemPaper::shape,ItemPaper::offset);
	}

    ItemPaper* newIt = new ItemPaper;

    newIt->loadAssets(ItemPaper::baseNode,ItemPaper::shape);

    return newIt;
}

irr::core::vector3df ItemPaper::getOffset() {
	return ItemPaper::offset;
}

std::string ItemPaper::getInvName() {
	return std::string("Mysterious Note");
}

std::string ItemPaper::getInvImgPath() {
	return std::string("GFX/Items/INVpaper.jpg");
}

bool ItemPaper::updateItem() {
	return true;
}

void ItemPaper::updateWearing() {}

void ItemPaper::drawItem() {
    irr::video::ITexture* docTex = Item::irrDriver->getTexture("GFX/Items/sn.it");
    unsigned short width = docTex->getSize().Width;
    unsigned short height = docTex->getSize().Height;
    float scale = Item::screenHeight/1024.f;
    Item::irrDriver->draw2DImage(docTex,irr::core::recti((Item::screenWidth/2)-(width*scale/2),(Item::screenHeight/2)-(height*scale/2),(Item::screenWidth/2)+(width*scale/2),(Item::screenHeight/2)+(height*scale/2)),irr::core::recti(0,0,width,height));
}
