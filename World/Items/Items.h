#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <irrlicht.h>
#include <irrDynamics.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <string>
#include "../GameObject.h"

enum class ItemTempIDs : unsigned short {
    ITEM_NULL,
    ITEM_420,
	ITEM_KEY1,
	ITEM_KEY2,
	ITEM_KEY3,
	ITEM_KEY4,
	ITEM_KEY5,
	ITEM_MISC,
	ITEM_KEY6,
	ITEM_SCP860,
	ITEM_PAPER,
	ITEM_VEST,
	ITEM_FINEVEST,
	ITEM_VERYFINEVEST,
	ITEM_HAZMATSUIT,
	ITEM_HAZMATSUIT2,
	ITEM_HAZMATSUIT3,
	ITEM_CUP,
	ITEM_EMPTYCUP,
	ITEM_SCP500,
	ITEM_FIRSTAID,
	ITEM_FINEFIRSTAID,
	ITEM_FIRSTAID2,
	ITEM_VERYFINEFIRSTAID,
	ITEM_GASMASK,
	ITEM_SUPERGASMASK,
	ITEM_GASMASK3,
	ITEM_BAT,
	ITEM_SCP148,
	ITEM_SCP148INGOT,
	ITEM_NAV,
	ITEM_RADIO,
	ITEM_FINERADIO,
	ITEM_VERYFINERADIO,
	ITEM_18VRADIO,
	ITEM_CIGARETTE,
	ITEM_420S,
	ITEM_HAND,
	ITEM_HAND2,
	ITEM_18VBAT,
	ITEM_KILLBAT,
	ITEM_FINEEYEDROPS,
	ITEM_SUPEREYEDROPS,
	ITEM_EYEDROPS,
	ITEM_SCP714,
	ITEM_SCP1025,
	ITEM_SCP513,
};

class World; class Player;

class Item : public GameObject {
    protected:
        irr::scene::ISceneNode* irrObj;
        btRigidBody* rbody;
        bool picked = true;
        float state[3];

        static void createShapeFromNode(irr::scene::IMeshSceneNode* node,btConvexHullShape* &outShape,irr::core::vector3df &offset);
        void loadAssets(irr::scene::IMeshSceneNode* node,btConvexHullShape* shape);

        Item();
        ~Item();
    public:
        virtual bool updateItem() =0;
		virtual void updateWearing() =0;
        virtual void drawItem() =0;
        virtual void setDrawCoords(irr::core::vector2di newCoords) =0;
        virtual std::string getInvName() =0;
        virtual std::string getInvImgPath() =0;
        virtual ItemTempIDs getTempID() =0;

        virtual float getState(int i) {
            return state[i];
        }
        virtual bool getPicked() {
            return picked;
        }

        virtual void pick();
        virtual void unpick(irr::core::vector3df position);

        virtual irr::core::vector3df getOffset() =0;

        virtual irr::core::aabbox3df getBBox();
        virtual irr::core::matrix4 getTransform();
        virtual irr::core::vector3df getPosition();

        enum class REFINE_SETTINGS : char {
            ROUGH,
            COARSE,
            RATIO1,
            FINE,
            VERYFINE
        };
        virtual bool refineItem(REFINE_SETTINGS setting,irr::core::aabbox3df intake,irr::core::aabbox3df output,Item*& result);

        virtual void destroy();
};

/*#include "ItemEyedrops/ItemEyedrops.h"
#include "ItemGasMask/ItemGasMask.h"*/
#include "Item420.h"
#include "ItemKey1.h"
#include "ItemKey2.h"
#include "ItemKey3.h"
#include "ItemKey4.h"
#include "ItemKey5.h"
#include "ItemKey6.h"
#include "ItemScp860.h"
#include "ItemPaper.h"
#include "ItemVest.h"
#include "ItemFinevest.h"
#include "ItemVeryfinevest.h"
#include "ItemHazmatsuit.h"
#include "ItemHazmatsuit2.h"
#include "ItemHazmatsuit3.h"
#include "ItemCup.h"
#include "ItemEmptycup.h"
#include "ItemScp500.h"
#include "ItemFirstaid.h"
#include "ItemFinefirstaid.h"
#include "ItemFirstaid2.h"
#include "ItemVeryfinefirstaid.h"
#include "ItemGasmask.h"
#include "ItemSupergasmask.h"
#include "ItemGasmask3.h"
#include "ItemBat.h"
#include "ItemMisc.h"
#include "ItemScp148.h"
#include "ItemScp148ingot.h"
#include "ItemNav.h"
#include "ItemRadio.h"
#include "ItemFineradio.h"
#include "ItemVeryfineradio.h"
#include "Item18vradio.h"
#include "ItemCigarette.h"
#include "Item420s.h"
#include "ItemHand.h"
#include "ItemHand2.h"
#include "Item18vbat.h"
#include "ItemKillbat.h"
#include "ItemFineeyedrops.h"
#include "ItemSupereyedrops.h"
#include "ItemEyedrops.h"
#include "ItemScp714.h"
#include "ItemScp1025.h"
#include "ItemScp513.h"

#endif // ITEMS_H_INCLUDED
