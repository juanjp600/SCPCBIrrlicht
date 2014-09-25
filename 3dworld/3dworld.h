#ifndef __3DWORLD_H_INCLUDED
#define __3DWORLD_H_INCLUDED

#include <algorithm>
#include <vector>
#include <iostream>

#include <irrlicht.h>
#include <irrDynamics.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "items/items.h"
#include "rooms/room.h"

const float RoomScale = 0.75f;

class MainEventReceiver : public irr::IEventReceiver {
    public:
        virtual bool OnEvent(const irr::SEvent& event) {
            if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
                KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown; /*{
                if (event.EventType == irr::EET_KEY_INPUT_EVENT&&!event.KeyInput.PressedDown)
                    switch(event.KeyInput.Key) {
                        case irr::KEY_KEY_1:
                        case irr::KEY_KEY_2:
                        case irr::KEY_KEY_3:
                    }
                */

                return true;
                //}
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
                        //PrevMousePosition = MousePosition;
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
        virtual bool IsMouseDown(unsigned char keyCode) const {
            return MouseIsDown[keyCode];
        }
        virtual irr::core::position2di getMousePos() const {
            return MousePosition;
        }

        MainEventReceiver() {
            memset(KeyIsDown, false, sizeof(KeyIsDown));
        }

    private:
        bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
        bool MouseIsDown[2];
        irr::core::position2di MousePosition;
        //irr::core::position2di PrevMousePosition;
};

class RoomShaderCallBack : public irr::video::IShaderConstantSetCallBack {
    public:

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,
            irr::s32 userData)
    {
        irr::video::IVideoDriver* driver = services->getVideoDriver();

        irr::core::matrix4 worldViewProj;
        worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);
        worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
        worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

        services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

        irr::s32 TextureLayerID = 0;
        services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
        irr::s32 TextureLayerID2 = 1;
        services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);
        irr::s32 TextureLayerID3 = 2;
        services->setPixelShaderConstant("Texture2", &TextureLayerID3, 1);
    }
};

class NormalsShaderCallBack: public irr::video::IShaderConstantSetCallBack {
    private:

    struct sortHelper {
        irr::video::SLight light;
        irr::f32 dist;
        bool operator < (const sortHelper &other) const {
            return (dist<other.dist);
        }
    };

    std::vector<sortHelper> lightList;
    public:

    irr::video::SColorf fvAmbient;
    void setLights(const std::vector<irr::video::SLight> &inList) {
        lightList.resize(inList.size());
        for (unsigned int i=0;i<lightList.size();i++) {
            sortHelper lig;
            lig.light = inList[i];
            lig.light.Radius *= lig.light.Radius;
            lig.dist = 0;
            lightList[i] = lig;
        }
    };

    void sortLights(irr::core::matrix4 transfrm) {
        for (unsigned int i=0;i<lightList.size();i++) {
            irr::core::vector3df pos = lightList[i].light.Position;
            transfrm.transformVect(pos);
            lightList[i].dist = pos.getLength();
        }
        std::sort(lightList.begin(),lightList.end());
    }

    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,
            irr::s32 userData)
    {
        irr::video::IVideoDriver* driver = services->getVideoDriver();

        irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
        invWorld.makeInverse();

        irr::core::matrix4 worldViewProj;
        worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
        worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

        irr::u32 cnt = lightList.size();

        irr::core::matrix4 lightTransform = worldViewProj*invWorld;

        sortLights(lightTransform);

		for (irr::u32 i=0; i<4; ++i)
		{
            std::string lightStrength = "lightRadius";
			lightStrength+=std::to_string(i+1);
			std::string lightPosition = "lightPos";
			lightPosition+=std::to_string(i+1);
			std::string lightColor = "lightColor";
			lightColor+=std::to_string(i+1);

			irr::video::SLight light;

			if (i<cnt)
				light = lightList[i].light;
			else
			{
				light.DiffuseColor.set(0,0,0);
				light.Radius = 1.0f;
			}

			light.DiffuseColor.a = 1.0f;

			lightTransform.transformVect(light.Position);

			services->setVertexShaderConstant(lightStrength.c_str(), (float*)(&light.Radius), 1);
			services->setVertexShaderConstant(lightPosition.c_str(), (float*)(&light.Position), 3);
			services->setVertexShaderConstant(lightColor.c_str(), (float*)(&light.DiffuseColor), 4);
		}

        irr::s32 TextureLayerID = 0;
        services->setPixelShaderConstant("baseMap", &TextureLayerID, 1);
        irr::s32 TextureLayerID1 = 1;
        services->setPixelShaderConstant("bumpMap", &TextureLayerID1, 1);
        irr::s32 TextureLayerID2 = 2;
        services->setPixelShaderConstant("specMap", &TextureLayerID2, 1);

		services->setPixelShaderConstant("ambientLight", (float*)(&fvAmbient), 4);
    }
};

class world {
    private:
        irr::video::E_DRIVER_TYPE irrDriverType;
        irr::IrrlichtDevice *irrDevice;
        irr::video::IVideoDriver* irrDriver;
        irr::scene::ISceneManager* irrSmgr;

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

        irr::s32 RoomShader, NormalsShader;
        RoomShaderCallBack* RoomCallback;
        NormalsShaderCallBack* NormalsCallback;

        std::vector<item*> itemList;

		room* roomArray[20][20];

        void createMap();
    public:
        //main
        world(unsigned int width,unsigned int height,bool fullscreen = false);
        ~world();

        float getFPSfactor();

        bool run();
};

const unsigned int inventory_size = 10;

class player {
    private:
        world* owner;
        irr::scene::ISceneManager* irrSmgr;
        irrDynamics* dynamics;
        MainEventReceiver* irrReceiver;
        float walkingSpeed;
        irr::scene::ICameraSceneNode* Camera;
        btRigidBody* Capsule;
        irr::core::vector3df selfRotation;

        item* inventory[inventory_size];

		float height,radius;

        float crouchState = 0.f;
        bool crouched = false;

        bool dead = false;
    public:
        player(world* own,irr::scene::ISceneManager* smgr,irrDynamics* dyn,MainEventReceiver* receiver,float height=28.0f,float radius=6.0f,float mass=10.0f);
        //mass should stay low if you want the player to be able the climb up stairs
        ~player();
        void update();
        float yaw = 0.f,pitch = 0.f,roll = 0.f;

        float BlinkTimer=100.0,Stamina=100.0;

        void addToInventory(item* it);
        void takeFromInventory(unsigned char slot);

        bool seesMeshNode(irr::scene::IMeshSceneNode* node);

        irr::scene::IMeshSceneNode* testNode;

        irr::core::vector3df getPosition() {
            return Camera->getPosition();
        }
};

#endif // 3DWORLD_H_INCLUDED
