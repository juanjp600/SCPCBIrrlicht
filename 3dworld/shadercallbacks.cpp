#include "shadercallbacks.h"
#include <iostream>

#include "rooms/rmesh.h"

void RoomShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 worldViewProj;
	/*worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);*/
	worldViewProj = driver->getTransform(irr::video::ETS_WORLD);

	services->setVertexShaderConstant("mWorld", worldViewProj.pointer(), 16);

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);
	irr::s32 TextureLayerID4 = 4;
	services->setPixelShaderConstant("Texture1_t", &TextureLayerID4, 1);
	irr::s32 TextureLayerID5 = 5;
	services->setPixelShaderConstant("Texture1_b", &TextureLayerID5, 1);
	irr::s32 TextureLayerID3 = 2;
	services->setPixelShaderConstant("Texture2", &TextureLayerID3, 1);
	irr::s32 TextureLayerIDNY = 3;
	services->setPixelShaderConstant("reflection", &TextureLayerIDNY, 1);

	services->setPixelShaderConstant("reflectFactor", &reflectFactor, 1);
}

void ZBufferShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
    irr::video::IVideoDriver* driver = services->getVideoDriver();
    irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_WORLD);

	services->setVertexShaderConstant("mWorld", worldViewProj.pointer(), 16);
}

void RoomShaderCallBack_noNormals::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 worldViewProj;
	/*worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);*/
	worldViewProj = driver->getTransform(irr::video::ETS_WORLD);

	services->setVertexShaderConstant("mWorld", worldViewProj.pointer(), 16);

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);
}

void VertLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
}

void PostProcShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	currmBlur += (minBlur-currmBlur) * 0.1f * fpsFactor;
	currMBlur += (maxBlur-currMBlur) * 0.1f * fpsFactor;

	services->setVertexShaderConstant("minBlur", &currmBlur, 1);
	services->setVertexShaderConstant("maxBlur", &currMBlur, 1);

	services->setVertexShaderConstant("gammaFactor", &invGammaFactor, 1);

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);

	irr::s32 TextureLayerIDFog = 2;
	services->setPixelShaderConstant("fogTex", &TextureLayerIDFog, 1);
}

void NormalsShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
	invWorld.makeInverse();

	irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

	irr::u32 cnt = lightList.size();

	irr::core::matrix4 lightTransform = worldViewProj*invWorld;

	sortLights(lightTransform);

	for (irr::u32 i=0;i<4;++i) {
		std::string lightPosition = "lightPos";
		lightPosition+=std::to_string(i+1);
		std::string lightColor = "lightColor";
		lightColor+=std::to_string(i+1);

		sortHelper light;

		if (i<cnt) {
			light = lightList[i];
		} else {
			light.color = irr::video::SColorf(0.f,0.f,0.f,1.f);
			light.pos[0]=0.f; light.pos[1]=0.f;
			light.pos[2]=0.f; light.pos[3]=0.f;
		}

		light.color.a = 1.0f;

        irr::core::vector3df lightPos(light.pos[0],light.pos[1],light.pos[2]);
		lightTransform.transformVect(lightPos);
		light.pos[0]=lightPos.X; light.pos[1]=lightPos.Y; light.pos[2]=lightPos.Z;

		services->setVertexShaderConstant(lightPosition.c_str(), (float*)(light.pos), 4);
		services->setPixelShaderConstant(lightColor.c_str(), (float*)(&light.color), 4);
	}

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("baseMap", &TextureLayerID, 1);
	irr::s32 TextureLayerID1 = 1;
	services->setPixelShaderConstant("bumpMap", &TextureLayerID1, 1);
	irr::s32 TextureLayerID2 = 2;
	services->setPixelShaderConstant("specMap", &TextureLayerID2, 1);

	services->setPixelShaderConstant("ambientLight", (float*)(&ambient), 4);
}

void LightsShaderCallBack::setLights(const std::vector<pointLight> &inList) {
	lightList.resize(inList.size());
	for (unsigned int i=0;i<lightList.size();i++) {
		sortHelper lig;
		//lig.light = inList[i];
		lig.color = inList[i].color;
		lig.pos[0] = inList[i].position.X;
		lig.pos[1] = inList[i].position.Y;
		lig.pos[2] = inList[i].position.Z;
		lig.pos[3] = inList[i].radius*inList[i].radius;
		lig.intensity = inList[i].intensity;
		for (unsigned int j=0;j<6;j++) {
            lig.viewMatrix[j] = inList[i].viewMatrix[j];
		}
		lig.dist = 0;
		lightList[i] = lig;
	}
}

void LightsShaderCallBack::sortLights(irr::core::matrix4 transfrm) {
	for (unsigned int i=0;i<lightList.size();i++) {
		irr::core::vector3df pos(lightList[i].pos[0],lightList[i].pos[1],lightList[i].pos[2]);
		transfrm.transformVect(pos);
		lightList[i].dist = pos.getLength();
	}
	std::sort(lightList.begin(),lightList.end());
}

#if 0
void PlainLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
	invWorld.makeInverse();

	irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

	irr::u32 cnt = lightList.size();

	irr::core::matrix4 lightTransform = worldViewProj*invWorld;

	sortLights(lightTransform);

	for (irr::u32 i=0;i<4;++i) {
		std::string lightPosition = "lightPos";
		lightPosition+=std::to_string(i+1);
		std::string lightColor = "lightColor";
		lightColor+=std::to_string(i+1);

		sortHelper light;

		if (i<cnt) {
			light = lightList[i];
		} else {
			light.color = irr::video::SColorf(0.f,0.f,0.f,1.f);
			light.pos[0]=0.f; light.pos[1]=0.f;
			light.pos[2]=0.f; light.pos[3]=0.f;
		}

		light.color.a = 1.0f;

        irr::core::vector3df lightPos(light.pos[0],light.pos[1],light.pos[2]);
		lightTransform.transformVect(lightPos);
		light.pos[0]=lightPos.X; light.pos[1]=lightPos.Y; light.pos[2]=lightPos.Z;

		services->setVertexShaderConstant(lightPosition.c_str(), (float*)(light.pos), 4);
		services->setPixelShaderConstant(lightColor.c_str(), (float*)(&light.color), 4);
	}

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("baseMap", &TextureLayerID, 1);

	services->setPixelShaderConstant("ambientLight", (float*)(&ambient), 4);
}
#endif

#if 1
void PlainLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
	invWorld.makeInverse();

	irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

	irr::u32 cnt = lightList.size();

	irr::core::matrix4 lightTransform = worldViewProj*invWorld;

	//sortLights(lightTransform);

	//for (irr::u32 i=0;i<4;++i) {
    std::string lightPosition = "lightPos1";
    std::string lightColor = "lightColor1";

    services->setVertexShaderConstant("lightMatrix", (lightMatrix*driver->getTransform(irr::video::ETS_WORLD)).pointer(), 16);

    sortHelper light = lightList[pass];

    light.color.a = 1.0f;

    /*light.color.r *= light.intensity;
    light.color.g *= light.intensity;
    light.color.b *= light.intensity;*/

    irr::core::vector3df lightPos(light.pos[0],light.pos[1],light.pos[2]);
    lightTransform.transformVect(lightPos);
    light.pos[0]=lightPos.X; light.pos[1]=lightPos.Y; light.pos[2]=lightPos.Z;
    //light.pos[3]*=100.f;

    services->setVertexShaderConstant(lightPosition.c_str(), (float*)(light.pos), 4);
    services->setPixelShaderConstant(lightColor.c_str(), (float*)(&light.color), 4);
	//}

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("renderedLights", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("lightDepthMap", &TextureLayerID2, 1);

	services->setPixelShaderConstant("ambientLight", (float*)(&ambient), 4);

	float TBN[3];
	TBN[0] = useTBN.X; TBN[1] = useTBN.Y; TBN[2] = useTBN.Z;
	services->setPixelShaderConstant("useTBN", TBN, 3);
}
#endif
