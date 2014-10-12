#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <irrlicht.h>
#include <irrDynamics.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <string>

enum class itemTempIDs : unsigned short {
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

class world;

class item {
    protected:
        irr::scene::ISceneNode* irrObj;
        btRigidBody* rbody;
        bool picked = true;
        float state[3];
        static dynRegister* dynamics;

        static void createShapeFromNode(irr::scene::IMeshSceneNode* node,btConvexHullShape* &outShape,irr::core::vector3df &offset);
        void loadAssets(irr::scene::IMeshSceneNode* node,btConvexHullShape* shape);
    public:
        virtual bool updateItem() =0;
        virtual std::string getInvName() =0;
        virtual itemTempIDs getTempID() =0;

        virtual float getState(int i) {
            return state[i];
        }
        virtual bool getPicked() {
            return picked;
        }

        virtual void Pick();
        virtual void Unpick(irr::core::vector3df position);

        virtual irr::core::vector3df getOffset() =0;

        virtual irr::core::aabbox3df getBBox();
        virtual irr::core::matrix4 getTransform();

        static void setDynamics(dynRegister* dyn);

        item();
        ~item();
};

/*#include "itemEyedrops/itemEyedrops.h"
#include "itemGasMask/itemGasMask.h"*/
#include "item420/item420.h"
#include "itemKey1/itemKey1.h"
#include "itemKey2/itemKey2.h"
#include "itemKey3/itemKey3.h"
#include "itemKey4/itemKey4.h"
#include "itemKey5/itemKey5.h"
#include "itemKey6/itemKey6.h"
#include "itemScp860/itemScp860.h"
#include "itemPaper/itemPaper.h"
#include "itemVest/itemVest.h"
#include "itemFinevest/itemFinevest.h"
#include "itemVeryfinevest/itemVeryfinevest.h"
#include "itemHazmatsuit/itemHazmatsuit.h"
#include "itemHazmatsuit2/itemHazmatsuit2.h"
#include "itemHazmatsuit3/itemHazmatsuit3.h"
#include "itemCup/itemCup.h"
#include "itemEmptycup/itemEmptycup.h"
#include "itemScp500/itemScp500.h"
#include "itemFirstaid/itemFirstaid.h"
#include "itemFinefirstaid/itemFinefirstaid.h"
#include "itemFirstaid2/itemFirstaid2.h"
#include "itemVeryfinefirstaid/itemVeryfinefirstaid.h"
#include "itemGasmask/itemGasmask.h"
#include "itemSupergasmask/itemSupergasmask.h"
#include "itemGasmask3/itemGasmask3.h"
#include "itemBat/itemBat.h"
#include "itemMisc/itemMisc.h"
#include "itemScp148/itemScp148.h"
#include "itemScp148ingot/itemScp148ingot.h"
#include "itemNav/itemNav.h"
#include "itemRadio/itemRadio.h"
#include "itemFineradio/itemFineradio.h"
#include "itemVeryfineradio/itemVeryfineradio.h"
#include "item18vradio/item18vradio.h"
#include "itemCigarette/itemCigarette.h"
#include "item420s/item420s.h"
#include "itemHand/itemHand.h"
#include "itemHand2/itemHand2.h"
#include "item18vbat/item18vbat.h"
#include "itemKillbat/itemKillbat.h"
#include "itemFineeyedrops/itemFineeyedrops.h"
#include "itemSupereyedrops/itemSupereyedrops.h"
#include "itemEyedrops/itemEyedrops.h"
#include "itemScp714/itemScp714.h"
#include "itemScp1025/itemScp1025.h"
#include "itemScp513/itemScp513.h"

#endif // ITEMS_H_INCLUDED
