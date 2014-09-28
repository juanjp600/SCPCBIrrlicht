#ifndef POCKETDIMENSION_ROOM_INCLUDED
#define POCKETDIMENSION_ROOM_INCLUDED

class pocketdimension : public room {
	private:
		static RMesh* baseRMesh;
		pocketdimension() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static pocketdimension* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
