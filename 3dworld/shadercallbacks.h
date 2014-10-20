#ifndef SHADERCALLBACKS_H_INCLUDED
#define SHADERCALLBACKS_H_INCLUDED

#include "irrlicht.h"
#include <vector>
#include <algorithm>

class RoomShaderCallBack : public irr::video::IShaderConstantSetCallBack {
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
};

class ZBufferShaderCallBack : public irr::video::IShaderConstantSetCallBack {
    public:

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {};
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
		void setLights(const std::vector<irr::video::SLight> &inList);

		void sortLights(irr::core::matrix4 transfrm);

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};

class LightsShaderCallBack: public irr::video::IShaderConstantSetCallBack {
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
		void setLights(const std::vector<irr::video::SLight> &inList);

		void sortLights(irr::core::matrix4 transfrm);

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData);
};


#endif // CALLBACKS_H_INCLUDED
