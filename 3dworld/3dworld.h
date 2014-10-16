#ifndef __3DWORLD_H_INCLUDED
#define __3DWORLD_H_INCLUDED

#include <algorithm>
#include <vector>
#include <iostream>

#include <AL/al.h>

#include <irrlicht.h>

#include <CGUITTFont.h>

#include <irrDynamics.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "items/items.h"
#include "rooms/room.h"
#include "../sound/soundWrapper.h"

#include "shadercallbacks.h"

const float RoomScale = 0.75f;

class MainEventReceiver : public irr::IEventReceiver {
	private:
        bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
        bool MouseIsDown[2];

        bool PrevKeyIsDown[irr::KEY_KEY_CODES_COUNT];
        irr::core::position2di MousePosition;
    public:
        virtual bool OnEvent(const irr::SEvent& event) {
            if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
                KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

                return true;
            } else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
                switch(event.MouseInput.Event) {
                    case irr::EMIE_LMOUSE_PRESSED_DOWN:
                        MouseIsDown[0] = true;
                    break;

                    case irr::EMIE_RMOUSE_PRESSED_DOWN:
                        MouseIsDown[1] = true;
                    break;

                    case irr::EMIE_LMOUSE_LEFT_UP:
                        MouseIsDown[0] = false;
                    break;

                    case irr::EMIE_RMOUSE_LEFT_UP:
                        MouseIsDown[1] = false;
                    break;

                    case irr::EMIE_MOUSE_MOVED:
                        MousePosition.X = event.MouseInput.X;
                        MousePosition.Y = event.MouseInput.Y;
                    break;

                    default:

                    break;
                }
            }
            return false;
        }

        virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
            return KeyIsDown[keyCode];
        }
        virtual bool IsPrevKeyDown(irr::EKEY_CODE keyCode) const {
            return PrevKeyIsDown[keyCode];
        }
        virtual void CopyToPrevKeys() {
            memcpy(PrevKeyIsDown,KeyIsDown,sizeof(PrevKeyIsDown));
        }
        virtual bool IsMouseDown(unsigned char keyCode) const {
            return MouseIsDown[keyCode];
        }
        virtual irr::core::position2di getMousePos() const {
            return MousePosition;
        }

        MainEventReceiver() {
            memset(KeyIsDown, false, sizeof(KeyIsDown));
            memset(PrevKeyIsDown, false, sizeof(PrevKeyIsDown));
            memset(MouseIsDown, false, sizeof(MouseIsDown));
            MousePosition = irr::core::position2di(0,0);
        }
};

class world {
    private:
        irr::video::E_DRIVER_TYPE irrDriverType;
        irr::IrrlichtDevice *irrDevice;
        irr::video::IVideoDriver* irrDriver;
        irr::scene::ISceneManager* irrSmgr;

		irr::scene::ISceneCollisionManager* irrColl;

        irr::gui::IGUIEnvironment* irrEnv;

        irr::io::IFileSystem* irrFileSystem;

        irr::ITimer* irrTimer;

        unsigned int mainWidth,mainHeight;

        MainEventReceiver* irrReceiver;

        irrDynamics* dynamics;

        class player* mainPlayer;

        irr::video::ITexture* blurImage;
        irr::video::ITexture* blurImage2;
        unsigned char blurAlpha = 0;

        irr::video::ITexture* BlinkMeterIMG;
        irr::video::ITexture* StaminaMeterIMG;

        irr::u32 prevTime = 0;
        float FPSfactor = 1;

        irr::s32 RoomShader, NormalsShader, LightsShader;
        RoomShaderCallBack* RoomCallback;
        NormalsShaderCallBack* NormalsCallback;
        LightsShaderCallBack* LightsCallback;

        std::vector<item*> itemList;
        irr::scene::IMeshSceneNode* genItemNode(const std::string &meshPath,const std::string &texPath,float scale);

		room* roomArray[20][20];

        void createMap();
        room* addRandomRoom(unsigned short x,unsigned short y,roomTypes type,char angle,int zone);

        irr::gui::CGUITTFont* font1;

        int ppx,ppy;

        sound* ambient[3];

        unsigned char menusOpen = 0;

        static const unsigned char PAUSEOPEN;
        static const unsigned char INVOPEN;
        irr::video::ITexture* invImgs[10];
    public:
        //main
        world(unsigned int width,unsigned int height,bool fullscreen = false);
        ~world();

        float getFPSfactor();

        bool run();

        unsigned char pickPlayerTriangle();
};

//#include "player.h"

inline int coordToRoomGrid(float coord);

bool getNodeTriangleTextureName(irr::scene::ISceneNode* node,const irr::core::triangle3df& tri,std::string& texname);

void trimFileName(std::string &inStr);

#endif // 3DWORLD_H_INCLUDED
