#ifndef ROOM2TOILETS_ROOM_INCLUDED
#define ROOM2TOILETS_ROOM_INCLUDED

class room2toilets : public room {
	private:
		static RMesh* baseRMesh;
		room2toilets() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2toilets* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
