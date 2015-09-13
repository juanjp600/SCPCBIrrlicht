#ifndef SHADERCALLBACKS_H_INCLUDED
#define SHADERCALLBACKS_H_INCLUDED

#include "irrlicht.h"
#include <vector>
#include <algorithm>

class SharedShaderCallBack : public irr::video::IShaderConstantSetCallBack {
    protected:
        void setFogConstants(irr::video::IMaterialRendererServices* services);
    public:
		static irr::video::SColorf ambientLight;
		static irr::video::SColorf addColor;
		static irr::scene::ICameraSceneNode* camera;
        static float fogNear;
        static float fogFar;
        //static irr::video::ITexture* fogTexture;
};

class RoomShaderCallBack : public SharedShaderCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);

		irr::f32 reflectFactor = 1.f;
};

class RoomShaderCallBack_noNormals : public SharedShaderCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class VertLightShaderCallBack : public SharedShaderCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class PostProcShaderCallBack : public SharedShaderCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
		irr::f32 minBlur = 0.f;
		irr::f32 maxBlur = 15.f;
		//irr::f32 fpsFactor = 1.f;
		irr::f32 currmBlur = 0.f;
		irr::f32 currMBlur = 15.f;

		irr::f32 gammaFactor = 1.f;
		irr::f32 invGammaFactor = 1.f;
};

class ZBufferShaderCallBack : public SharedShaderCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class LightsShaderCallBack: public SharedShaderCallBack {
    protected:

	public:

        struct sortHelper {
            bool prioritize;
            irr::video::SColorf color;
            irr::f32 pos[4];
            irr::f32 intensity;
			//irr::video::SLight light;
			irr::f32 dist;
			bool operator < (const sortHelper &other) const {
				return (dist<other.dist);
			}
			//irr::core::matrix4 viewMatrix[6];
		};

        static std::vector<sortHelper> lightList;

		static void setLights(const std::vector<class pointLight> &inList,unsigned int prioritize);

		static void sortLights(irr::core::vector3df nodePos);

		//virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class PlainLightShaderCallBack: public LightsShaderCallBack {
	public:
		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);

		unsigned char pass = 0;
		irr::core::matrix4 lightMatrix;
		irr::core::vector3df useTBN;
};

class NormalsShaderCallBack: public LightsShaderCallBack {
	public:
		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};



#endif // CALLBACKS_H_INCLUDED
