#include "itemPaper.h"

irr::scene::IMeshSceneNode* itemPaper::baseNode = nullptr;
btConvexHullShape* itemPaper::shape = nullptr;
irr::core::vector3df itemPaper::offset = irr::core::vector3df(0,0,0);

itemPaper::itemPaper() {}

void itemPaper::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemPaper::baseNode == nullptr) {
        itemPaper::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemPaper* itemPaper::createItemPaper() {
    if (itemPaper::baseNode == nullptr) return nullptr;

    if (itemPaper::shape == nullptr) {
		item::createShapeFromNode(itemPaper::baseNode,itemPaper::shape,itemPaper::offset);
	}

    itemPaper* newIt = new itemPaper;

    newIt->loadAssets(itemPaper::baseNode,itemPaper::shape);

    return newIt;
}

irr::core::vector3df itemPaper::getOffset() {
	return itemPaper::offset;
}

std::string itemPaper::getInvName() {
	return std::string("Mysterious Note");
}

std::string itemPaper::getInvImgPath() {
	return std::string("GFX/items/INVpaper.jpg");
}

bool itemPaper::updateItem() {
	return true;
}

void itemPaper::updateWearing() {}

void itemPaper::drawItem() {
    irr::video::ITexture* docTex = item::irrDriver->getTexture("GFX/items/sn.it");
    unsigned short width = docTex->getSize().Width;
    unsigned short height = docTex->getSize().Height;
    float scale = item::screenHeight/1024.f;
    item::irrDriver->draw2DImage(docTex,irr::core::recti((item::screenWidth/2)-(width*scale/2),(item::screenHeight/2)-(height*scale/2),(item::screenWidth/2)+(width*scale/2),(item::screenHeight/2)+(height*scale/2)),irr::core::recti(0,0,width,height));
}
