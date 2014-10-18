#include "shadercallbacks.h"

void RoomShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
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

void PostProcShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	//irr::video::IVideoDriver* driver = services->getVideoDriver();

	/*irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

	services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);*/

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
	/*irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);
	irr::s32 TextureLayerID3 = 2;
	services->setPixelShaderConstant("Texture2", &TextureLayerID3, 1);*/
}

void NormalsShaderCallBack::setLights(const std::vector<irr::video::SLight> &inList) {
	lightList.resize(inList.size());
	for (unsigned int i=0;i<lightList.size();i++) {
		sortHelper lig;
		lig.light = inList[i];
		lig.light.Radius *= lig.light.Radius;
		lig.dist = 0;
		lightList[i] = lig;
	}
}

void NormalsShaderCallBack::sortLights(irr::core::matrix4 transfrm) {
	for (unsigned int i=0;i<lightList.size();i++) {
		irr::core::vector3df pos = lightList[i].light.Position;
		transfrm.transformVect(pos);
		lightList[i].dist = pos.getLength();
	}
	std::sort(lightList.begin(),lightList.end());
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

void LightsShaderCallBack::setLights(const std::vector<irr::video::SLight> &inList) {
	lightList.resize(inList.size());
	for (unsigned int i=0;i<lightList.size();i++) {
		sortHelper lig;
		lig.light = inList[i];
		lig.light.Radius *= lig.light.Radius;
		lig.dist = 0;
		lightList[i] = lig;
	}
}

void LightsShaderCallBack::sortLights(irr::core::matrix4 transfrm) {
	for (unsigned int i=0;i<lightList.size();i++) {
		irr::core::vector3df pos = lightList[i].light.Position;
		transfrm.transformVect(pos);
		lightList[i].dist = pos.getLength();
	}
	std::sort(lightList.begin(),lightList.end());
}

void LightsShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
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

	services->setPixelShaderConstant("ambientLight", (float*)(&fvAmbient), 4);
}