#ifndef __WORLD_H_INCLUDED
#define __WORLD_H_INCLUDED

#include <algorithm>
#include <vector>
#include <iostream>

#include <AL/al.h>

#include <irrlicht.h>

#include <CGUITTFont.h>

#include <irrDynamics.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "Items/Items.h"
#include "Rooms/Room.h"
#include "../Sound/SoundWrapper.h"

#include "ShaderCallbacks.h"

const float RoomScale = 0.2f;

//extern ContactAddedCallback gContactAddedCallback;
bool CustomMaterialCombinerCallback(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1);

class MainEventReceiver : public irr::IEventReceiver {
	private:
        bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
        bool MouseIsDown[2];

        bool PrevKeyIsDown[irr::KEY_KEY_CODES_COUNT];
        bool PrevMouseIsDown[2];

        bool MouseDoubleClick[2];
        irr::core::position2di MousePosition;

        irr::u32 lastMouseClick;

        irr::u32 time;
        irr::u32 time1[2],time2[2];
    public:
		void setTime(irr::u32 inTime) {
			time=inTime;
		}

        virtual bool OnEvent(const irr::SEvent& event) {
            if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
                KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

                return true;
            } else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
                switch(event.MouseInput.Event) {
                    case irr::EMIE_LMOUSE_PRESSED_DOWN:
                        MouseIsDown[0] = true;
                        time2[0]=time1[0]; time1[0]=time;
                        if (time1[0]-time2[0]<=500) { MouseDoubleClick[0]=true; }
                    break;

                    case irr::EMIE_RMOUSE_PRESSED_DOWN:
                        MouseIsDown[1] = true;
                        time2[1]=time1[1]; time1[1]=time;
                        if (time1[1]-time2[1]<=500) MouseDoubleClick[1]=true;
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
                return true;
            }
            return false;
        }

        virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
            return KeyIsDown[keyCode];
        }
        virtual bool IsPrevKeyDown(irr::EKEY_CODE keyCode) const {
            return PrevKeyIsDown[keyCode];
        }
        virtual bool IsMouseDown(unsigned char keyCode) const {
            return MouseIsDown[keyCode];
        }
        virtual bool IsPrevMouseDown(unsigned char keyCode) const {
            return PrevMouseIsDown[keyCode];
        }
        virtual bool IsDoubleClick(unsigned char keyCode) const {
            return MouseDoubleClick[keyCode];
        }
        virtual irr::core::position2di getMousePos() const {
            return MousePosition;
        }

		void perLoopUpdate() {
			MouseDoubleClick[0]=MouseDoubleClick[1]=false;
			memcpy(PrevKeyIsDown,KeyIsDown,sizeof(PrevKeyIsDown));
            memcpy(PrevMouseIsDown,MouseIsDown,sizeof(PrevMouseIsDown));
		}

        MainEventReceiver() {
            memset(KeyIsDown, false, sizeof(KeyIsDown));
            memset(PrevKeyIsDown, false, sizeof(PrevKeyIsDown));
            memset(MouseIsDown, false, sizeof(MouseIsDown));
            memset(PrevMouseIsDown, false, sizeof(PrevMouseIsDown));
            MousePosition = irr::core::position2di(0,0);
        }
};

struct TempPathList {
    TempPathList* prev = nullptr;
    unsigned char x,y;
    TempPathList* next = nullptr;
};

struct TempWPPathList {
    TempWPPathList* prev = nullptr;
    unsigned char index;
    TempWPPathList* next = nullptr;
};

class World {
    private:
        irr::video::E_DRIVER_TYPE irrDriverType;
        irr::IrrlichtDevice* irrDevice;
        irr::video::IVideoDriver* irrDriver;
        irr::scene::ISceneManager* irrSmgr;
		irr::scene::ISceneCollisionManager* irrColl;
        irr::gui::IGUIEnvironment* irrEnv;
        irr::io::IFileSystem* irrFileSystem;
        irr::ITimer* irrTimer;
        irr::u32 time;

        unsigned int mainWidth,mainHeight;

        MainEventReceiver* irrReceiver;

        irrDynamics* dynamics;

        class Player* mainPlayer;
        class NPC* testNPC[15];

        irr::core::vector2di marked;

        irr::video::IRenderTarget reflection = irr::video::IRenderTarget(0);

        irr::video::IRenderTarget blurImage = irr::video::IRenderTarget(0);
        irr::video::IRenderTarget blurImage2 = irr::video::IRenderTarget(0);
        irr::video::IRenderTarget finalImage = irr::video::IRenderTarget(0);
        irr::scene::IMeshSceneNode* screenQuad;
        irr::video::IRenderTarget zBuffer = irr::video::IRenderTarget(0);

        irr::core::array<irr::video::IRenderTarget> deferredLightData;
        irr::core::array<irr::video::IRenderTarget> renderedLights;
        irr::scene::IMeshSceneNode* deferredLightSphere;

        //irr::video::ITexture* lightPass[2];

        irr::video::IRenderTarget fogTexture = irr::video::IRenderTarget(0);

        //irr::video::ITexture* lightDepth;

        unsigned char blurAlpha = 0;

        irr::video::ITexture* blinkMeterIMG;
        irr::video::ITexture* staminaMeterIMG;

        irr::u32 prevTime = 0;
        //float fpsFactor = 1;

        irr::video::E_MATERIAL_TYPE roomShader, roomShader_noNormals, vertLightShader, vertLightShader_alpha,
                                    fogBillboardShader, normalsShader, plainLightShader, renderDeferredLightShader,
                                    postProcShader, zBufferShader;

        void shadersSetup();
        void setupForHWSkinning(irr::scene::IAnimatedMesh* mesh);
        void setupForPlainLighting(irr::scene::ISceneNode* node);

        RoomShaderCallBack* roomCallback;
        RoomShaderCallBack_noNormals* roomCallback_noNormals;
        VertLightShaderCallBack* vertLightCallback;
        NormalsShaderCallBack* normalsCallback;
        PlainLightShaderCallBack* plainLightCallback;
        RenderDeferredLightShaderCallBack* renderDeferredLightCallback;
        PostProcShaderCallBack* postProcCallback;
        ZBufferShaderCallBack* zBufferCallback;

        std::vector<Item*> itemList;
        irr::scene::IMeshSceneNode* genItemNode(const std::string &meshPath,const std::string &texPath,float scale);

        std::vector<class Door*> doorList;

		Room* roomArray[20][20];

        void createMap(unsigned char zone);
        unsigned char currentZone = 0;
        Room* addRandomRoom(unsigned short x,unsigned short y,RoomTypes type,char angle,int zone);

        //pathfinding
        short stepPath(const irr::core::vector2di &endPos,TempPathList &roomPath,int depth=0);

        irr::gui::CGUITTFont* font1;
        irr::gui::CGUITTFont* font2;

        int ppx,ppy;

        Sound* ambient[3];

		enum class menus {
			NONE,PAUSEOPEN,INVOPEN,OPTIONSOPEN
		};

		menus menusOpen = menus::NONE; unsigned char subMenusOpen = 0;
        menus prevMenusOpen = menus::NONE;

        irr::video::ITexture* pauseImgs[3];
        irr::video::ITexture* invImgs[10];
        bool itemSelected = false;

        bool button(const std::string &text,int x,int y,int w,int h);

        float scale2D = 1.f;
        float scale2Db = 1.f;

        unsigned char dragItem = 10;

        void draw3D();
        void drawFog();
        void drawHUD();

        irr::scene::IBillboardSceneNode* fogBillboards[50];
        irr::core::vector3df fogBillTargetPos[50];
        //float fogBillAlpha[50];

        std::string hudMsg = "";
        float hudMsgTimer = 0.f;

        void renderLights();

        irr::scene::ICameraSceneNode* smCamera;

        std::vector<irr::core::vector2di> drawHandIcon;

        float stepsToMake = 0.f;
    public:
        //main
        World(unsigned int width,unsigned int height,bool fullscreen = false);
        ~World();

        //float getFPSfactor();

        bool run();

        unsigned char pickPlayerTriangle(irr::core::vector3df* intersec = nullptr,const irr::core::vector3df customEnd = (irr::core::vector3df(0.f,-204.8*RoomScale,0.f)));
        void getRoomList(const irr::core::vector2di &startPos,const irr::core::vector2di &endPos,std::vector<irr::core::vector2di> &roomPath);
        void getRoomListToPlayer(const irr::core::vector2di &startPos,std::vector<irr::core::vector2di> &roomPath);
        void npcPathFind(const irr::core::vector3df &startPos,const irr::core::vector3df &endPos,const irr::core::vector2di &RoomPos,std::vector<irr::core::vector3df> &posList);
        void npcPathFindToPlayer(const irr::core::vector3df &startPos,const irr::core::vector2di &RoomPos,std::vector<irr::core::vector3df> &posList);
};

struct SSkinningVertex
{
	SSkinningVertex()
	{
		for (irr::u32 i = 0; i < 4; ++i)
		{
			BlendWeight[i] = 0.f;
			BlendIndex[i] = 0.f;
		}
	}

	SSkinningVertex(const SSkinningVertex& other)
	{
		Pos = other.Pos;
		Normal = other.Normal;
		TCoords = other.TCoords;

		memcpy(BlendWeight, other.BlendWeight, sizeof(irr::f32) * 4);
		memcpy(BlendIndex, other.BlendIndex, sizeof(irr::f32) * 4);
	}

	SSkinningVertex& operator=(const SSkinningVertex& other)
	{
		Pos = other.Pos;
		Normal = other.Normal;
		TCoords = other.TCoords;

		memcpy(BlendWeight, other.BlendWeight, sizeof(irr::f32) * 4);
		memcpy(BlendIndex, other.BlendIndex, sizeof(irr::f32) * 4);

		return *this;
	}

	bool operator==(const SSkinningVertex& other) const
	{

		return ((Pos == other.Pos) && (Normal == other.Normal) && (TCoords == other.TCoords) &&
				memcmp(&BlendWeight, other.BlendWeight, sizeof(irr::f32) * 4) == 0 &&
				memcmp(&BlendIndex, other.BlendIndex, sizeof(irr::f32) * 4) == 0);
	}

	irr::core::vector3df Pos;
	irr::core::vector3df Normal;
	irr::core::vector2d<irr::f32> TCoords;
	irr::f32 BlendWeight[4];
	irr::f32 BlendIndex[4];
};

//#include "Player.h"

extern int coordToRoomGrid(float coord);

bool getNodeTriangleTextureName(irr::scene::ISceneNode* node,const irr::core::triangle3df& tri,std::string& texname);

void trimFileName(std::string &inStr);

#endif // World_H_INCLUDED
