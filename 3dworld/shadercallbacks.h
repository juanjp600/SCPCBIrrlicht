#ifndef SHADERCALLBACKS_H_INCLUDED
#define SHADERCALLBACKS_H_INCLUDED

#include "irrlicht.h"
#include <vector>
#include <algorithm>

class RoomShaderCallBack : public irr::video::IShaderConstantSetCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);

		irr::f32 reflectFactor = 1.f;
};

class RoomShaderCallBack_noNormals : public irr::video::IShaderConstantSetCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class VertLightShaderCallBack : public irr::video::IShaderConstantSetCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class PostProcShaderCallBack : public irr::video::IShaderConstantSetCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
		irr::f32 minBlur = 0.f;
		irr::f32 maxBlur = 15.f;
		irr::f32 fpsFactor = 1.f;
		irr::f32 currmBlur = 0.f;
		irr::f32 currMBlur = 15.f;

		irr::f32 gammaFactor = 1.f;
		irr::f32 invGammaFactor = 1.f;
};

class ZBufferShaderCallBack : public irr::video::IShaderConstantSetCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class LightsShaderCallBack: public irr::video::IShaderConstantSetCallBack {
    protected:

		struct sortHelper {
            irr::video::SColorf color;
            irr::f32 pos[4];
			//irr::video::SLight light;
			irr::f32 dist;
			bool operator < (const sortHelper &other) const {
				return (dist<other.dist);
			}
		};

		std::vector<sortHelper> lightList;
	public:

		irr::video::SColorf ambient;
		void setLights(const std::vector<irr::video::SLight> &inList);

		void sortLights(irr::core::matrix4 transfrm);

		//virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class PlainLightShaderCallBack: public LightsShaderCallBack {
	public:
		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class NormalsShaderCallBack: public LightsShaderCallBack {
	public:
		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};



#endif // CALLBACKS_H_INCLUDED
